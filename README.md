# **HARD DISK "VIRTUALE"**

  Piccolo progetto fatto più come passatempo che come utilità vera e propria

## DESCRIZIONE
La struttura dati sector rappresenta un settore dell'hard disk che è totalmente personalizzabile tramite le due macro presenti all'interno del file [``hd.hpp``](hd.hpp).
La struttura interface simula una possibile interfaccia del dispositivo che è il modo in cui la CPU parla con il dispositivo
non ostante per ora non siano implementati l'interfaccia è già stata predisposta per gli hard disk a doppia densità e gli hard disk
su più livelli (ovvero quelli con più di un piatto che tipicamente si indica con il numero del cilindro a cui appartengono).


## Funzioni di utilità 
| Nome          | Firma                                                                     |   
|:-------------:|:-------------------------------------------------------------------------:|
| **ReadHD**    |``` bool readHD(void* buff, size_t len, u_int64_t n_sect);       ```       |
| **WriteHD**   |``` bool writeHD(const void* const buff,size_t len,size_t mode = APPEND)```|
| **LoadHD**    |``` bool loadHD(const char* const path = "./hd.txt");```                   |
| **SaveHD**    |``` bool saveHD(const char* const path = "./hd.txt");```                   |
| **WriteTest** |``` void writeTest(void);```                                               |
| **PrintHD**   |``` void printHD(const size_t mode = ALL_SECT);```                         |

### readHD 
```cpp
  bool readHD(void* buff,size_t len,u_int64_t n_sect);
```

La funzione readHD è una funziona di utilità che serve per leggere una grandezza len di un settore nsect ovviamente questa cosa è puramente una "magia del software" in quanto un hard disk reale è in gradi di leggere scrivere interi settori e non porzioni di essi  


### WriteHD
```cpp
  bool writeHD(const void* const buff,size_t len,size_t mode = APPEND);
```

Funzione di utilità che legge len byte dal disco in una delle due modalità definite append o no append.

| Nome      | Descrizione                                                                                      |
|:---------:|:------------------------------------------------------------------------------------------------:|
| APPEND    | Cerca il primo spazio libero all'interno del settore(posizione libera = 0) e scrive len byte.    |
| NO_APPEND | Scrive partendo dalla prima posizione del settore sovrascrivendo eventualmente ciò che cè sopra. |

Nel caso in cui la funzione non riesce a scrive in un settore len byte l'operazione fallisce.


### LoadHD
```cpp
  bool loadHD(const char* const path = "./hd.txt");
```
Funzione di utilità che serve per caricare un contenuto precedenmente salvato nel file hd.txt nell'attuale istanza del disco.


**Attenzione:** Questa funzione apre il file passato come argomento in sola lettura quindi se il file non esiste la fopen restituisce un errore.

### SaveHD
```cpp
  bool saveHD(const char* const path = "./hd.txt");
```
Funzione di utilità che serve per fare la serializzazione dello stato attuale del disco.
Alla funzione viene passato come argomento il percorso del file se il file non esiste il file viene creato.

### WriteTest
```cpp
  void writeTest(void);
```
Funzione che consente di scrivere tutto il contenuto del disco con tutti 1 utile in fase di test.


### PrintHD
```cpp
  void printHD(const size_t mode = ALL_SECT);
```
Funzione che consente di stampare il contenuto del disco secondo 4 diverse modalità.
I 4 byte più bassi di mode sono stati usati per discriminare la modalità di stampa i restanti nel caso la modalità lo supporti contengono il numero del settore da stampare

| Modalità   | Descrizione                       |
|:----------:|:---------------------------------:|
| ALL_SECT   | Stampa tutti i settori del disco  |
| FIRST_SECT | Stampa il primo settore del disco |
| N_SECT     | Stampa l'n-esimo settore del disco|
| LAST_SECT  | Stampa l'ultimo settore del disco |

## COSTANTI

Le macro vengono elencate in ordine di immissione nel codice la colonna funzione indica la funzione dove vengono utilizzate

#### Nota : Le funzioni elencate sono prive di argomenti per una questione di spazio e leggibilità della documentazione, inoltre vengono ripetute anche le macro della funzione printHD() precedentemente descritte per un fatto di completezza


| Nome           | Valore | Funzione         | Descrizione                                                                                                                              |
|:--------------:|:------:|:----------------:|:----------------------------------------------------------------------------------------------------------------------------------------:|
| APPEND         | 0x00   |``` writeHD()```  | Se questa valore è settato allora la scrittura avviene in append all'interno del settore desiderato                                      |
| NO_APPEND      | 0x01   |``` writeHD()```  | Se questa valore è settato allora la scrittura avviene partendo dall'inizio del settore                                                  |
| BIT_START      | 0x20   |``` inth()```     | Indica il bit di start necessario per far partire il trasferimento mediante l'interfaccia                                                |
| BIT_READ       | 0x10   |``` inth()```     | Indica il commando di lettura tramite l'interfaccia                                                                                      |
| BIT_WRITE      | 0x08   |``` inth()```     | Indica il commando di scrittura tramite l'interfaccia                                                                                    |
| ERR_READ       | 0x04   |``` inth()```     | Se questo valore si trova dentro il registro di stato vuol dire che si è verificato un errore durante la lettura                         |
| ERR_WRITE      | 0x03   |``` inth()```     | Indica il commando di scrittura tramite l'interfaccia                                                                                    |
| ERR_MOD_UNKOWN | 0x05   |``` inth()```     | Se questo valore si trova dentro il registro di stato vuol dire che la modalità precedentemente settata non è supportata dall'interfaccia|
| ALL_SECT       | 0x00   |``` printHD()```  | Stampa l'intero disco                                                                                                                    |
| FIRST_SECT     | 0x01   |``` printHD()```  | Stampa il primo settore del disco                                                                                                        |
| N_SECT         | 0x02   |``` printHD()```  | Stampa l'n-esimo settore del disco                                                                                                       |
| LAST_SECT      | 0x04   |``` printHD()```  | Stampa l'ultimo settore del disco                                                                                                        |

# INTERFACCIA
