#! /bin/bash
rm *.aux *.log *.out *.synctex.gz *.toc
git add .
git commit -m "update"
git push
