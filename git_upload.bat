@echo off
@REM ssh -T git@github.com
git add .
git commit -m "%date%"
git push