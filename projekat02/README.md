# Projektni zadatak 2

## Zadatak 1):

U ovom delu zadatka napisana je funkicija **sampleAndHold** koja vrši interpolaciju slike **Sample and hold** algoritmom.

![formula Sample and hold algoritma](./media/sample_and_hold.png)

**Slika 1** – Formula **Sample and hold** algoritma

> ![lena](./media/lena.bmp)
> ![lena - sample and hold](./media/lena_sample_and_hold.jpeg)

**Slika 2** – Rezultat **sample and hold** interpolacije za parametre 2, 2

## Zadatak 2):

U ovom delu zadatka napisana je funkicija **bilinearInterpolate** koja vrši interpolaciju slike koja vrši bilinearnu interpolaciju slike algoritmom.

![formula bilinearne interpolacije](./media/bilinear.png)

**Slika 3** – Formula za bilinearnu interpolaciju

> ![lena](./media/lena.bmp)
> ![lena - bilinear](./media/lena_bilinear.jpeg)

**Slika 4** – Rezultat **bilinearne** interpolacije za paramtre 2, 2

## Zadatak 3):

U ovom delu zadatka napisana je funkicija **bicubicInterpolate** koja vrši interpolaciju slike koja vrši bikubičnu interpolaciju slike.

![formula bikubicne interpolacije](./media/bicubic.png)

**Slika 5** – Formula **bikubične** interpolacije

> ![lena](./media/lena.bmp)
> ![lena - bicubic](./media/lena_bicubic.jpeg)

**Slika 6** – Rezultati **bikubične** interpolacije za parametre 2, 2

## Zadatak 4):

U ovom delu zadatka napisana je funkicija **imageSwirl** koja vrši **Swirl** transformaciju slike, za parametar **k**, po sledećoj formuli.

![swirl formula](./media/swirld.png)

**Slika 7** – Formula **Swirl** algoritma

> ![lena](./media/lena.bmp)
> ![lena - swirl](./media/lena_swirl.jpeg)

**Slika 8** – Rezultat **Swirl** algoritma za parametar 0.005

## Zadatak 5):

U ovom delu zadatka napisana je funkicija **imageSwirlBilinear** koja vrši **Swirl** transformaciju slike, za parametar **k**, međutim koristi bilinearnu formulu za računanje vrednosti piksela.


> ![lena](./media/lena.bmp)
> ![lena - bilinear swirl](./media/lena_bilinear_swirl.jpeg)

**Slika 9** – Rezultat **SwirlBilinear** algoritma za parametar 0.005

## Bonus:

Nakon urađene **Swirl** transformacije za 0.005 urađena je **Swirl** transformacija za -0.005. Zbog ograničenja u veličini slike jedan deo slike izgubljen je nakon rekonstrukcije. Rezultati nakon rekonstrukcije za **Swirl** i **SwirlBilinear** dati su ispod.

> ![lena](./media/lena.bmp)
> ![lena - swirl](./media/lena_swirl.jpeg)

**Slika 10** – Originalna slika (levo) i slika nakon primene **Swirl** transformacije za parametar 0.005 (desno)

> ![lena remade with swirl](./media/lena_remade_swirl.jpg)
> ![lena remade with bilinear swirl](./media/lena_remade_bilinear_swirl.jpg)

**Slika 11** – Slika nakon rekonstrukcije **Swirl** algoritom (levo) I
**SwirlBilinear** algoritmom (desno) za parametar -0.005
