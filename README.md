# Projekt: Izdelava programskega jezika

## Avtor
Nejc Mozetič

## Opis projekta
To je moj projekt za maturo iz informatike leta 2024. Projekt zajema razvoj preprostega programskega jezika, ki omogoča osnovne programske operacije, kot so izrazi, spremenljivke, izpisovanje, pogojni stavki, zanke in funkcije. Koda je prevedena v **assembly**, ki neposredno operira z registri, skladom in osnovnimi operacijami na računalniku.

## Struktura projekta
- `prevajalnik.cpp` - C++ program, ki prevaja programski jezik v **assembly**.
- `porocilo.pdf` - Dokumentacija projekta, ki opisuje delovanje jezika in implementacijo prevajalnika.
- `out.asm` - Generirana assembly koda.

## Funkcionalnosti jezika
### 1. Osnovni izrazi
Podprte operacije:
- Seštevanje: `+`
- Odštevanje: `-`
- Množenje: `*`
- Deljenje: `/`
- Oklepaji: `()`

Primer:
```plaintext
1 + 2 * 3 * (5 - 4)
```

### 2. Spremenljivke
Dodelitev spremenljivke:
```plaintext
a = 5
b = a + 3
```

### 3. Izpisovanje vrednosti
Uporaba `pisi`:
```plaintext
pisi 5
pisi a + 2
```

### 4. Pogojni stavki
Podprti operatorji: `<`, `>`, `<=`, `>=`, `=`
```plaintext
ce a > 5
  pisi a
sicer
  pisi 5
konec
```

### 5. Zanke
#### `dokler` zanka:
```plaintext
a = 0
dokler a < 5
  pisi a
  a = a + 1
konec
```
#### `za` zanka:
```plaintext
za a do 5
  pisi a
konec
```

### 6. Funkcije
Deklaracija funkcije:
```plaintext
fun sestej a b
  vrni a + b
konec
```
Klic funkcije:
```plaintext
sestej 3 5
pisi sestej
```

## Uporaba prevajalnika
1. Prevajanje kode:
   ```sh
   g++ -o prevajalnik prevajalnik.cpp
   ```
2. Zagon prevajalnika:
   ```sh
   ./prevajalnik koda.txt
   ```
3. Izvajanje prevedene `assembly` kode (po prevodu v `out.asm`):
   ```sh
   nasm -f elf64 out.asm && ld -o out out.o && ./out
   ```

## Licenca
Ta projekt je odprtokoden in na voljo za uporabo v izobraževalne namene.

