# Projektni zadatak 1

##  Zadatak 1):

U ovom delu zadatka napisane su funkcije **calculateShelvingCoeff** i
**calculatePeekCoeff** koje računaju koeficijente all-pass filtera
prvog, odnosno drugog reda, po sledećim formulama:

![](./media/image2.png)

**Slika 1** – Formule za izračunavanje koeficijenata

Kako se koeficijenti A1 i B1 nalaze na opsegu \[-2, 2), na njihivim
položajima čuvaće se polovina njihove vrednosti, tako da će raspored u
memoriji izgledati ovako:

![](./media/image3.png)

**Slika 2** – Izgled koeficijenata u nizu

## Zadatak 2):

Za potrebe ovog zadatka napisana je funkcija **shelvingLP**, koja za
računanje koristi **first\_order\_iir**. Impulsni odzivi na filter sa
karakteristikom **α = 0.3**, sa koeficijentima **K= 8192** (0.25) i **K
= 24576** (0.75) dat je na grafiku ispod.

![](./media/image4.jpeg)

**Slika 3** – Impulsni odziv, **shelvingLP** (α = 0.3, K= 8192)

![](./media/image5.jpeg)

**Slika 4** – Impulsni odziv, **shelvingLP** (α = 0.3, K= 24576)

## Zadatak 3):

Za potrebe ovog zadatka napisana je funkcija **shelvingHP**, koja za
računanje koristi **first\_order\_iir**. Impulsni odzivi na filter sa
karakteristikom **α = -0.3**, sa koeficijentima **K= 8192** (0.25) i **K
= 24576** (0.75) dat je na grafiku ispod.

![](./media/image6.jpeg)

**Slika 5** – Impulsni odziv, **shelvingHP** (α = -0.3, K= 8192)

![](./media/image7.jpeg)

**Slika 6** – Impulsni odziv, **shelvingHP** (α = -0.3, K= 24576)

## Zadatak 4):

Za potrebe ovog zadatka napisana je funkcija **shelvingPeek**, koja za
računanje koristi **second\_order\_iir**. Impulsni odzivi na filter sa
karakteristikom **α = 0.7**, **β = 0**, sa koeficijentima **K= 8192**
(0.25) i **K = 24576** (0.75) dat je na grafiku ispod.

![](./media/image8.jpeg){width="6.502083333333333in"
height="1.0583333333333333in"}

**Slika 7** – Impulsni odziv, **shelvingPeek** (α = 0.7, β = 0, K= 8192)

![](./media/image9.jpeg)

**Slika 8** – Impulsni odziv, **shelvingPeek** (α = 0.7, β = 0, K= 24576)

**Zadatak 5):**
---------------

Ovaj zadatak predstavlja proširenje i uniju prethodna 4 zadatka. Pre
obrade signala računaju se koeficijenti α i β za svaki od delova
ekvilajzera, izračunati tako da odgovaraju frekvencijama koje su date u
tabeli.

  Band 0 f<sub>C</sub>  | Band 1 f<sub>0</sub> | Band 1 Δf | Band 2 f<sub>0</sub> | Band 2 Δf | Band 3 f<sub>C</sub>
  -------------|-------------|-----------|-------------|-----------|-------------
  140 Hz       |390 Hz       |200 Hz     |2935 Hz      |1905 Hz    |5500 Hz

**Tabela 1** – Vrednosti frekvencija

![](./media/image10.png)

**Slika 9** – Formula za izračunavanje koeficijenata α i β

![](./media/image11.png)

**Slika 10 –** Izlaz sistema na impulsni odziv

![](./media/image12.png)

**Slika 11 –** Ulazni signal u dati sistem
(multiton)

![](./media/image13.png)

**Slika 12** – Izaz sistema na multiton signal za različite vrednosti parametara k u sve 4 faze

**Zadatak 6):**
---------------

Za potrebe ovog zadatka uveden je niz koeficijenata k, koji sadrži 4
elementa. Svaki od elemenata predstavlja k za jednu od 4 faze iz
prethodnog zadatka. Vrednost ovih koeficijenata može da se menja
pritiskom na tastere **SW1** i **SW2**, za čije potrebe je napisana
funkcija **getKey**. Trenutno stanje ispisuje se nakon svake promene na
displej. Prvi karakter predstavlja indeks broja k
koji se trenutno prikazuje i menja (0-3), a drugi predstavlja vrednost
selektiranog k. k može uzeti vrednost između 0.1 i 1.0, koje odgovaraju
vrednostima od 3277 do 32767. Za potrebe ispisa dodata su 2 karaktera u
tabelu (`" "` i `"."`) i realizovana je funkcija **printNewValue**, koja
ispisuje nove vrednosti nakon svake promene.
