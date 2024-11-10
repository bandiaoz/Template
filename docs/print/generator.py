import os
import glob

rootDir = "../../src/"

sections = [o for o in os.listdir(rootDir) if os.path.isdir(os.path.join(rootDir, o))]
sections.sort()
print(sections) # ['DataStruct', 'Geometry', 'Graph', 'Math', 'Misc', 'String', 'Tree']

with open("contents.tex", "w") as fp:
    for section in sections:
        if section == ".git":
            continue
        fp.write(f"\\section{{{section}}}\n")
        for root, dirs, files in os.walk(os.path.join(rootDir, section)):
            for f in files:
                if f == ".DS_Store":
                    continue
                escaped = f.replace("_", "\\_")
                f = os.path.relpath(os.path.join(root, f), rootDir)
                print(f)
                fp.write(f"\\subsection{{{escaped}}}\n")
                fp.write(
                    f"\\lstinputlisting[style=cpp]{{../../src/{f}}}\n"
                )

# 编译 main.tex
os.system("xelatex main.tex")
os.system("xelatex main.tex")
os.system("rm *.aux *.log *.out *.toc")