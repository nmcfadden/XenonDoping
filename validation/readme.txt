MaGe Validation

Before MaGe validation can be used it needs to be compiled using the script
"build.sh". Afterwards the validation can be executed via ".\MaGeValidation.py".
As soon as the validation is finished a PDF report file "report.pdf" containing
the results of the validation is available.

G4GDML is needed for validation.  MaGe should be compiled with G4GDML enabled.  

The script MaGeValidation.py will create temporary files that can take ~ 10 GB
of disk space.  These files will be deleted when validation is finished.

$MAGEDIR/lib must be included in LD_LIBRARY_PATH (or DYLD_LIBRARY_PATH, for
Mac).

