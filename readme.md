# Dino Game
Saioc Marius-Andrei

##Descriere
Rolul proiectului este implementarea unui joculet, Dino, dupa modelul lui Chrome Dino (Dinosaur Game), la nivel de Arduino, afisarea fiind efectuata pe un LCD 2X16.

##Componente
Ansamblul este format din:
-Placa de Dezvoltare Compatibila cu Arduino UNO R3 (ATmega328p + ATmega16u2) 
-LCD 1602 cu Interfata I2C
-Modul Joystick Biaxial
-Breadboard
-Fire


##Mod de ultilizare
Jocul este controlat prin Joystick. Pentru a porni jocul (la inceput de rulare sau dupa game over) se foloseste directia sus a joystick-ului. Pe parcursul jocului folosim directia sus pentru jump si jos pentru crouch, cu scopul de a evita obstacolele.
Aditional se poate folosi directia stanga pentru a pune pauza in orice moment al jocului, iar dreapta pentru a reveni dintr-o pauza.


##Mod de functionare
La prima rulare, se intra intr-o bucla care verifica pozitia joystick-ului pe verticala, asteptand pozitia sus pentru a porni jocul.
In functia de loop fiecare obiect este tratat individual dupa cum urmeaza:

Pozitia dino-ului este comandata de 2 varibile dinoUp si dinoDown, care marcheaza daca dino sare (dinoUp==1), este pe crouch (dinoDown==1) sau este intr-o pozitie normala, cand ambele variabile sunt false.
Actualizarea dino-ului pe LCD se face la fiecare 50ms, fiind desenat intr-una din cele 3 pozitii in functie de valorile variabilelor de stare.
Stare de crouch este activa atata timp cat joystick-ul este mentinut in jos; starea de jump este insa limitata, dino fiind obligat sa cada jos la fiecare saritura dupa ce trece un interval de timp invers proportional cu viteza de deplasare a obstacolelor.

In orice moment va exista o scriere pentru cactus si pasare (chiar daca pozitia lor nu e momentan pe ecran). La inceputul jocului cactusul va fi pozitionat pe pozitia 15, iar pasarea undeva in spatele acestuia cu un numar random de casute.
Pozitia acestora este actualizata cu 1 la un interval de timp (care porneste de la 500ms si scade din 2 in 2 puncte obtinute pana la un minim de 200ms). Cand un obiect ajunge la capatul din stanga al ecranului, reprezentat de indicele -1, acesta revine in partea din dreapta,
cactusul revenind la o pozitie fixa (15), iar pasarea la una random. Se face de asemenea verificarea ca pozitiile celor 2 obstacole sa nu se suprapuna si sa existe cel putin o casuta libera intre cele 2. Putem avea un cactus sau doi unul langa altul (ales random).

La intersectia obstacolelor cu dino, se verifica starea lui dino. La intersectia cactusului dino trebuie sa fie in aer (dinoUp==true), iar la cea cu pasarea sa fie pe crouch (dinoDown==0), pentru a primi punctajul si a continua jocul. Altfel, inseamna ca a avut loc o ciocnire
si va aparea ecranul de gameOver, jocul putand fi resetat de pe joystick sus.


##Biblioteci folosite
- Wire.h  		- permite comunicarea cu dipozitive I2C/TWI, LCD-ul avand interfata I2C
- hd44780.h		- biblioteca open source, folosita pentru functiile de scriere pe LCD
- hd44780ioClass/hd44780_I2Cexp.h   - biblioteca expander care adapteaza functiile la LCD cu interfata I2C