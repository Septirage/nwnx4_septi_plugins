#!/usr/bin/env python3
"""Build a guide PDF from a Markdown file, with a generated table of contents
(with page numbers).

Usage: build_guide_pdf.py guide.md [out.pdf]

The markdown must contain a `<!-- TOC -->` marker where the table of contents
goes. The stylesheet is tool/guide-style.css, shared by all the guides (a guide
that needs a different setting puts a <style> block at its top). A line
`<!-- INCLUDE other.md -->` is replaced by the content of that file (path
relative to the markdown), so a file such as CHANGELOG.md can be the single
source for both the guide and the website. Headings of level 1-3
are listed. A `<!-- MINITOC -->` marker is replaced by the list of the level 4
headings that follow it (up to the next heading of level 1-3), with their page:
a small table of contents for one section. Each level 4 section (its heading and
its text) is wrapped in a `<div class="keep">`, so it is not split across two
pages. A line made only of bold text (such as a version line of a changelog)
becomes a `<p class="boldline">`, for the stylesheet. The title and the author of
the PDF are taken from the cover (`cover-heading`, and `cover-version` such as
"V1.2.0 – 2026-10-17 by Septirage"). The build runs in a temporary copy so nothing is written next to
the sources.

Requires: md-to-pdf, pdftotext (poppler).
Two passes: the first renders with placeholder page numbers, we then read the
page of each heading in the PDF, and render again. The TOC keeps the same size,
so pagination does not move between the two passes.
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile

MAX_LEVEL = 3
STYLESHEET = os.path.join(os.path.dirname(os.path.abspath(__file__)), "guide-style.css")
MINI_LEVEL = 4


def slug(title):
    # same rule as the markdown renderer used by md-to-pdf (marked)
    s = title.lower()
    s = re.sub(r"[^\w\- ]", "", s)
    return s.replace(" ", "-")


def expand_includes(md, basedir):
    def repl(m):
        with open(os.path.join(basedir, m.group(1).strip()), encoding="utf-8") as f:
            return f.read().rstrip() + "\n"
    return re.sub(r"<!--\s*INCLUDE\s+(.+?)\s*-->", repl, md)


def headings(md, max_level=MAX_LEVEL):
    out = []
    in_code = False
    for line in md.splitlines():
        if line.startswith("```"):
            in_code = not in_code
        if in_code:
            continue
        m = re.match(r"^(#{1,%d}) +(.+?) *$" % max_level, line)
        if m:
            out.append((len(m.group(1)), m.group(2).replace("`", "")))
    return out


def toc_html(entries, pages):
    rows = []
    for level, title in entries:
        n = pages.get(title, "")
        rows.append(
            '<li class="l%d"><a href="#%s"><span>%s</span><span class="dots"></span>'
            "<span>%s</span></a></li>" % (level, slug(title), title, n)
        )
    return '<ul class="toc-list">\n' + "\n".join(rows) + "\n</ul>"


def mark_bold_lines(md):
    out = []
    in_code = False
    for line in md.splitlines():
        if line.startswith("```"):
            in_code = not in_code
        m = None if in_code else re.match(r"^\*\*([^*]+)\*\*\s*$", line)
        if m:
            text = re.sub(r"`([^`]+)`", r"<code>\1</code>", m.group(1))
            line = '<p class="boldline"><strong>%s</strong></p>' % text
        out.append(line)
    return "\n".join(out) + "\n"


def keep_level4_together(md):
    out = []
    in_code = False
    opened = False
    for line in md.splitlines():
        if line.startswith("```"):
            in_code = not in_code
        m = None if in_code else re.match(r"^(#{1,%d}) " % MINI_LEVEL, line)
        if m and opened:
            out += ["", "</div>", ""]
            opened = False
        if m and len(m.group(1)) == MINI_LEVEL:
            out += ['<div class="keep">', ""]
            opened = True
        out.append(line)
    if opened:
        out += ["", "</div>"]
    return "\n".join(out) + "\n"


def minitoc_html(titles, pages):
    rows = []
    for title in titles:
        rows.append(
            '<li><a href="#%s"><span>%s</span><span class="dots"></span>'
            "<span>%s</span></a></li>" % (slug(title), title, pages.get(title, ""))
        )
    # filled column by column: the script gives the number of rows
    rows_nb = max(1, (len(titles) + 1) // 2)
    return ('<ul class="minitoc" style="grid-template-rows: repeat(%d, auto)">\n' % rows_nb
            + "\n".join(rows) + "\n</ul>")


def expand_minitocs(md, pages):
    parts = md.split("<!-- MINITOC -->")
    out = parts[0]
    for part in parts[1:]:
        titles = []
        for level, title in headings(part, MINI_LEVEL):
            if level <= MAX_LEVEL:
                break
            titles.append(title)
        out += minitoc_html(titles, pages) + part
    return out


def render(workdir, md_name):
    subprocess.run(
        ["md-to-pdf", "--stylesheet", "guide-style.css", md_name],
        cwd=workdir, check=True, stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL, timeout=180,
    )
    return os.path.join(workdir, os.path.splitext(md_name)[0] + ".pdf")


def page_texts(pdf):
    n = int(re.search(r"Pages:\s+(\d+)", subprocess.run(
        ["pdfinfo", pdf], capture_output=True, text=True, check=True).stdout).group(1))
    texts = []
    for p in range(1, n + 1):
        t = subprocess.run(["pdftotext", "-f", str(p), "-l", str(p), "-layout", pdf, "-"],
                           capture_output=True, text=True, check=True).stdout
        texts.append([ln.strip() for ln in t.splitlines()])
    return texts


def find_pages(entries, texts):
    # headings are searched in document order, each one from the page of the
    # previous one, so a line equal to a title elsewhere is not taken
    pages = {}
    start = 1  # page 1 is the cover + TOC
    for _, title in entries:
        for i in range(start, len(texts)):
            if title in texts[i]:
                pages[title] = i + 1
                start = i
                break
        else:
            print("warning: heading not found in the PDF: %s" % title, file=sys.stderr)
    return pages


def cover_info(md):
    heading = re.search(r'<div class="cover-heading">(.+?)</div>', md)
    version = re.search(r'<div class="cover-version">(.+?)</div>', md)
    if not heading:
        return None, None
    title, author = heading.group(1).strip(), None
    if version:
        title += " " + version.group(1).split()[0]
        if " by " in version.group(1):
            author = version.group(1).rsplit(" by ", 1)[1].strip()
    return title, author


def set_pdf_info(pdf, title, author):
    # Chromium names the PDF after the local URL and gives no author: append an
    # incremental update with a new Info dictionary (the rest of the file is kept)
    def pdf_text(text):
        return b"<FEFF" + text.encode("utf-16-be").hex().upper().encode() + b">"

    with open(pdf, "rb") as f:
        data = f.read()
    tail = data[data.rindex(b"trailer"):]
    size = int(re.search(rb"/Size (\d+)", tail).group(1))
    root = re.search(rb"/Root (\d+ \d+ R)", tail).group(1)
    prev = int(re.search(rb"startxref\s+(\d+)", tail).group(1))
    entries = b""
    info = re.search(rb"/Info (\d+) (\d+) R", tail)
    if info:
        old = re.search(rb"\n%s %s obj\s*<<(.*?)>>\s*endobj" % (info.group(1), info.group(2)), data, re.S)
        if old:
            entries = re.sub(rb"/(Title|Author)\s*(\((?:\\.|[^\\)])*\)|<[0-9A-Fa-f]*>)", b"", old.group(1))
    entries += b"/Title " + pdf_text(title)
    if author:
        entries += b"/Author " + pdf_text(author)
    if not data.endswith(b"\n"):
        data += b"\n"
    offset = len(data)
    data += b"%d 0 obj\n<<%s>>\nendobj\n" % (size, entries)
    xref = len(data)
    data += b"xref\n%d 1\n%010d 00000 n \n" % (size, offset)
    data += b"trailer\n<</Size %d /Root %s /Info %d 0 R /Prev %d>>\nstartxref\n%d\n%%%%EOF\n" % (
        size + 1, root, size, prev, xref)
    with open(pdf, "wb") as f:
        f.write(data)


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    src = os.path.abspath(sys.argv[1])
    out = os.path.abspath(sys.argv[2]) if len(sys.argv) > 2 else os.path.splitext(src)[0] + ".pdf"
    srcdir, md_name = os.path.dirname(src), os.path.basename(src)
    md = expand_includes(open(src, encoding="utf-8").read(), srcdir)
    md = keep_level4_together(mark_bold_lines(md))
    if "<!-- TOC -->" not in md:
        sys.exit("no <!-- TOC --> marker in %s" % md_name)
    entries = headings(md)
    all_entries = headings(md, MINI_LEVEL)

    with tempfile.TemporaryDirectory() as tmp:
        work = os.path.join(tmp, "build")
        shutil.copytree(srcdir, work, ignore=shutil.ignore_patterns("*.pdf"))
        shutil.copyfile(STYLESHEET, os.path.join(work, "guide-style.css"))
        pages = {title: "00" for _, title in all_entries}
        for _ in range(2):
            with open(os.path.join(work, md_name), "w", encoding="utf-8") as f:
                f.write(expand_minitocs(md, pages).replace("<!-- TOC -->", toc_html(entries, pages)))
            pdf = render(work, md_name)
            pages = find_pages(all_entries, page_texts(pdf))
        title, author = cover_info(md)
        if title:
            set_pdf_info(pdf, title, author)
        shutil.copyfile(pdf, out)
    print("written", out)


if __name__ == "__main__":
    main()
