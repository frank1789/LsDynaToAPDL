[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Qt:5.9](https://img.shields.io/badge/Qt-5.9-brightgreen.svg)](https://www.qt.io)

# LsDynaToAPDL

##### Versione :it:

Questo progetto nasce nel 2015, durante lo sviluppo del progetto di tesi per la laurea triennale in ingegneria meccanica, consiste nello sviluppo di un programma per convertire il file di output di LsDyna nel formato APDL per Ansys e Workbench. Obiettivo principale conserva gli spessori variabili degli elementi oggetto dell'analisi.

##### Version :uk:

This project was born in 2015, during the development of the thesis for a bachelor degree in mechanical engineering. Consisting in developing a program to convert LsDyna output files to APDL files readable from Ansys and Workbench.

## Getting Started

##### Versione :it:

Allo stato attuale il software riconosce e gestisce unicamente i nodi e gli elementi di tipo shell sia in lettura dal formato Lsdyna, sia in scrittura nel formato APDL.
Si fa notare inoltre che gli elementi di tipo *shell* in scrittura sono di tipo: **SHELL181** codificati nel codice.
Le altre sezioni sono attualmente ignorate.

##### Version :uk:
At present, the software only recognizes and manages nodes and shell-type elements both in read from Lsdyna format and in write in APDL format.
It is also noted that the * shell * writing elements are of the type: ** SHELL181 ** is hardcoded.
The other sections are currently ignored.

### Prerequisites

##### Versione :it:

Conoscenza della sintassi LsDyna e sintassi APDL

##### Version :uk:

Knowledge of LsDyna syntax and APDL syntax

## Authors

* **Francesco Argentieri**

## License

This project is licensed under the GPL v3 License - see the [LICENSE](https://github.com/frank1789/LsDynaToAPDL/blob/master/LICENSE) file for details.
