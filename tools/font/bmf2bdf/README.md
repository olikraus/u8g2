bmf2bdf: Converter from .bmf to .bdf file format.

.bmf: See [http://bmf.php5.cz/](http://bmf.php5.cz/)

Features
  - Convert .bmf to .bdf, result will be written to stdout
  - Converts DOS codepage to Unicode (-u option)
  - Convert monospaced proportional font back to none-monospaced font (-p)
  - Optional glyph spacing with -x
  - Optional baseline adjustment with -y
  - Brightness threshold with -l
  - Generated BDF can be modified with gbdfed and FontForge
    (http://sofia.nmsu.edu/~mleisher/Software/gbdfed/, https://github.com/andrewshadura/gbdfed)
  
Example:
  [http://bmf.php5.cz/index.php?font=goth16](http://bmf.php5.cz/index.php?font=goth16)
```
    bmf2bdf -u -p -x 1  goth16.bmf > goth16.bdf
```
