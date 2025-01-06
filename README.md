# AP-Math + ESP32

## Note
Questo programma è stato sviluppato nell'ambito della presentazione degli studenti Tommaso Pellegrino, Alessandro Salerno,
Davide Vella del 21/03/2024 presso IIS A. Avogadro. Il codice distribuito nella directory `.pio/` **NON** è di proprietà degli sviluppatori di
questo software ed è soggetto a termini di licenza propri. Tutto il codice esterno incluso è **strettamente necessario** per il funzionamento del programma,
nello specifico, si tratta di librerie e strumenti legati alle piattaforme ESP-IDF ed Arduino per ESP32.

Il codice distribuito nelle directory `src/`, `include/`, `test/` e `lib/` è interamente di proprietà degli sviluppatori del software ed è distribuito privatamente per
analisi, sperimentazione e dimostrazione. Si richiede di contattare gli sviluppatori per ottenere permesso a ridistribuire il software. Questo software non ha assicurazioni o garanzie di alcun tipo.

La lingua inglese è stata usata nel codice per convenzioni ed estetica.

## AP-Math
AP-Math è una piccola libreria di Aritmetica a Precisione Arbitraria per valori interi che permette di effettuare operazioni di addizione e sottrazione di valori
positivi e negativi al di fuori dei limiti dimensionali imposti dalla piattaforma. L'interfaccia di AP-Math è dichiarata in `lib/apmath/src/apmath.hpp`, mentre
l'implementazione è definita in `lib/apmath/src/apmath.cpp`.

## Programma
Il resto del programma (in `src/main.cpp`) utilizza le funzionalità di AP-Math in modo interattivo mediante l'interfaccia seriale UART.

## Test
I test presenti in `test/test_main.cpp` permettono di verificare il corretto funzionamento di AP-Math sul modulo ESP32 mediante lo strumento di Unit-Testing `Unity` disponibile via PlatformIO.
