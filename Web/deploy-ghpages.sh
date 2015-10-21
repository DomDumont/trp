#!/bin/bash
(
cd _build/html
touch .nojekyll
git init
git config user.name "Travis-CI"
git config user.email "travis@travis.com"
git add .
git commit -m "Deployed to Github Pages"
git push --force --quiet "https://${GH_TOKEN}@github.com/DomDumont/trp.git" master:gh-pages > /dev/null 2>&1
)
