@echo off
set TAG_NAME=v0.1.7-Stable

echo Creating git tag %TAG_NAME%
git tag %TAG_NAME%
if errorlevel 1 (
    echo Failed to create tag %TAG_NAME%
    exit /b 1
)

echo Pushing tag %TAG_NAME% to remote
git push origin %TAG_NAME%
if errorlevel 1 (
    echo Failed to push tag %TAG_NAME% to remote
    exit /b 1
)

echo Tag %TAG_NAME% created and pushed successfully.
