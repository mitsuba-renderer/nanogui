How to make a new release?
--------------------------

1. Update version in ``include/nanogui/common.h``

2. Commit: ``git commit -am "vX.Y.Z release"``

3. Tag: ``git tag -a vX.Y.Z -m "vX.Y.Z release"``

4. Push: ``git push`` and ``git push --tags``

5. Build via GitHub actions (``Wheels`` target)

6. Upload via GitHub or ``twine upload <filename>``
