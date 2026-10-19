#!/usr/bin/env python3
"""Build the PDF of every plugin guide in one run.

Usage: build_all_guides.py [out_dir]

Builds each src/plugins/<plugin>/doc/<plugin>_Guide.md with build_guide_pdf.py,
and each function guide src/plugins/<plugin>/nssdocumentation/*_pdf.md with
md-to-pdf and the pdf-style.css next to it (given as content with --css: the
function guides keep the default md-to-pdf style, and add theirs on top).
The PDFs are written to out_dir (default: guides_pdf/ at the root of the
repository): <name of the markdown>.pdf for the admin guides,
<plugin>_functionGuide.pdf for the function guides. A guide that fails does not
stop the others; the script ends with an error code if one failed.
"""
import glob
import os
import shutil
import subprocess
import sys
import tempfile

TOOL_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_DIR = os.path.dirname(TOOL_DIR)
BUILD_ONE = os.path.join(TOOL_DIR, "build_guide_pdf.py")


def build_function_guide(md, out):
    # Same as: md-to-pdf --css "$(cat pdf-style.css)" xxx_pdf.md, in a temporary copy
    srcdir = os.path.dirname(md)
    with open(os.path.join(srcdir, "pdf-style.css"), encoding="utf-8") as f:
        css = f.read()
    with tempfile.TemporaryDirectory() as tmp:
        work = os.path.join(tmp, "build")
        shutil.copytree(srcdir, work, ignore=shutil.ignore_patterns("*.pdf"))
        name = os.path.basename(md)
        result = subprocess.run(["md-to-pdf", "--css", css, name], cwd=work,
                                stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL, timeout=600)
        if result.returncode != 0:
            return False
        shutil.copyfile(os.path.join(work, os.path.splitext(name)[0] + ".pdf"), out)
    print("written", out)
    return True


def main():
    out_dir = os.path.abspath(sys.argv[1]) if len(sys.argv) > 1 else os.path.join(REPO_DIR, "guides_pdf")
    os.makedirs(out_dir, exist_ok=True)

    guides = sorted(glob.glob(os.path.join(REPO_DIR, "src", "plugins", "*", "doc", "*_Guide.md")))
    if not guides:
        sys.exit("no guide found in src/plugins/*/doc/")

    failed = []
    for guide in guides:
        name = os.path.splitext(os.path.basename(guide))[0]
        out = os.path.join(out_dir, name + ".pdf")
        print("building", name, "...", flush=True)
        result = subprocess.run([sys.executable, BUILD_ONE, guide, out], stdin=subprocess.DEVNULL)
        if result.returncode != 0:
            failed.append(name)

    function_guides = sorted(glob.glob(os.path.join(REPO_DIR, "src", "plugins", "*", "nssdocumentation", "*_pdf.md")))
    for guide in function_guides:
        plugin = os.path.basename(os.path.dirname(os.path.dirname(guide)))
        name = plugin + "_functionGuide"
        print("building", name, "...", flush=True)
        try:
            ok = build_function_guide(guide, os.path.join(out_dir, name + ".pdf"))
        except (OSError, subprocess.SubprocessError):
            ok = False
        if not ok:
            failed.append(name)

    total = len(guides) + len(function_guides)
    print("%d guide(s) built in %s" % (total - len(failed), out_dir))
    if failed:
        sys.exit("failed: " + ", ".join(failed))


if __name__ == "__main__":
    main()
