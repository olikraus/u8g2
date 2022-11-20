# bmf2bdf: Converter from .bmf to .bdf file format.

.bmf: See [http://bmf.php5.cz/](http://bmf.php5.cz/)

Features
  - Convert .bmf to .bdf, result will be written to stdout
  - Converts DOS codepage to Unicode (-u option)
  - Convert monospaced proportional font back to none-monospaced font (-p)
  - Optional glyph spacing with -x
  - Optional baseline adjustment with -y
  - Brightness threshold with -l
  - Generated BDF can be modified with gbdfed and FontForge
  
Example:
```
    bmf2bdf -u -p -x 1  font.bmf > font.bdf
```
