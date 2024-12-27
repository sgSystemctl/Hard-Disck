# **HARD DISK "VIRTUALE"**

  Piccolo progetto fatto più come passatempo che come utilità vera e propria
  
  ## DESCRIZIONE
  La struttura dati sector rappresenta un settore dell'hard disk che è totalmente personalizzabile tramite le due macro presenti all'interno del file [``hd.hpp``](hd.hpp).
  La struttura interface simula una possibile interfaccia del dispositivo che è il modo in cui la CPU parla con il dispositivo
  non ostante per ora non siano implementati l'interfaccia è già stata predisposta per gli hard disk a doppia densità e gli hard disk
  su più livelli (ovvero quelli con più di un piatto che tipicamente si indica con il numero del cilindro a cui appartengono).


  ## Funzioni di utilità 
  | Nome        | Firma                                                |   
  |:-----------:|:----------------------------------------------------------------:|
  | ReadHD      |``` cpp bool readHD(void* buff, size_t len, u_int64_t n_sect); ``` |
  <tr><th>Nome</th>       <th>Firma</th></tr>
  <tr><td>ReadHD</td>     <td><code>bool readHD(void* buff,size_t len,u_int64_t n_sect);</code></td></tr>
  <tr><td>WriteHD</td>    <td><code>bool writeHD(const void* const buff,size_t len,size_t mode = APPEND)</code></td></tr>
  <tr><td>LoadHD</td>     <td><code>bool loadHD(const char* const path = "./hd.txt");</code></td></tr>
  <tr><td>SaveHD</td>     <td><code>bool saveHD(const char* const path = "./hd.txt");</code></td></tr>
  <tr><td>WriteTest</td>  <td><code>void writeTest(void);</code></td></tr>
  <tr><td>PrintHD</td>    <td><code>void printHD(const size_t mode = ALL_SECT);</code></td></tr>


  ### readHD 
  <code>bool readHD(void* buff,size_t len,u_int64_t n_sect);</code>
  <p>
    La funzione readHD è una funziona di utilità che serve per leggere una grandezza len di un settore nsect ovviamente questa cosa è puramente una "magia del software" in quanto un hard disk reale è in gradi di leggere scrivere interi settori e non porzioni di essi  
  </p>

  ### WriteHD
  <code>bool writeHD(const void* const buff,size_t len,size_t mode = APPEND)</code>
<p>
    Funzione di utilità che legge len byte dal disco in una delle due modalità definite append o no append.
</p>
<table>
  
  <tr><th>Nome</th>          <th>Descrizione</th></tr>
  <tr><td>APPEND</td>        <td>Cerca il primo spazio libero all'interno del settore(posizione libera = 0) e scrive len byte.</td></tr>
  <tr><td>NO_APPEND</td>     <td>Scrive partendo dalla prima posizione del settore sovrascrivendo eventualmente ciò che cè sopra.</td></tr>

</table>

<p>
  Nel caso in cui la funzione non riesce a scrive in un settore len byte l'operazione fallisce.
</p>

### LoadHD
<code>bool loadHD(const char* const path = "./hd.txt");</code>
<p>
    Funzione di utilità che serve per caricare un contenuto precedenmente salvato nel file hd.txt nell'attuale istanza del disco.
</p>

**Attenzione:** Questa funzione apre il file passato come argomento in sola lettura quindi se il file non esiste la fopen restituisce un errore.

### SaveHD
<code>bool saveHD(const char* const path = "./hd.txt");</code>
<p>
    Funzione di utilità che serve per fare la serializzazione dello stato attuale del disco.
    Alla funzione viene passato come argomento il percorso del file se il file non esiste il file viene creato.
</p>

### WriteTest
<code>void writeTest(void);</code>
<p>
    Funzione che consente di scrivere tutto il contenuto del disco con tutti 1 utile in fase di test.
</p>

### PrintHD
<code>void printHD(const size_t mode = ALL_SECT);</code>
<p>
    Funzione che consente di stampare il contenuto del disco secondo 4 diverse modalità.
    I 4 byte più bassi di mode sono stati usati per discriminare la modalità di stampa i restanti nel caso la modalità lo supporti contengono il numero del settore da stampare
</p>
<table>
    <tr><th>Modalità</th><th>Descrizione</th></tr>
    <tr><td>ALL_SECT</td>   <td>Stampa tutti i settori del disco</td></tr>
    <tr><td>FIRST_SECT</td> <td>Stampa il primo settore del disco</td></tr>
    <tr><td>N_SECT</td>     <td>Stampa l'n-esimo settore del disco</td></tr>
    <tr><td>LAST_SECT</td>  <td>Stampa l'ultimo settore del disco</td></tr>
</table>

## COSTANTI


Le macro vengono elencate in ordine di immissione nel codice la colonna funzione indica la funzione dove vengono utilizzate

#### Nota : Le funzioni elencate sono prive di argomenti per una questione di spazio e leggibilità della documentazione, inoltre vengono ripetute anche le macro della funzione printHD() precedentemente descritte per un fatto di completezza


<table>
  <tr><th>Nome</th> <th>Valore</th> <th>Funzione</th>                <th>Descrizione</th></tr>
  
  <tr><td>APPEND</td> <td>0x00</td> <td><code>writeHD()</code></td> <td>Se questa valore è settato allora la scrittura avviene in append all'interno del settore desiderato</td></tr>

  <tr><td>NO_APPEND</td> <td>0x01</td> <td><code>writeHD()</code></td> <td>Se questa valore è settato allora la scrittura avviene partendo dall'inizio del settore</td></tr>

  <tr><td>BIT_START</td> <td>0x20</td> <td><code>inth()</code></td> <td>Indica il bit di start necessario per far partire il trasferimento mediante l'interfaccia</td></tr>

  <tr><td>BIT_READ</td> <td>0x10</td> <td><code>inth()</code></td> <td>Indica il commando di lettura tramite l'interfaccia</td></tr>

  <tr><td>BIT_WRITE</td> <td>0x08</td> <td><code>inth()</code></td> <td>Indica il commando di scrittura tramite l'interfaccia</td></tr>

  <tr><td>ERR_READ</td> <td>0x04</td> <td><code>inth()</code></td> <td>Se questo valore si trova dentro il registro di stato vuol dire che si è verificato un errore durante la lettura</td></tr>

  <tr><td>ERR_WRITE</td> <td>0x03</td> <td><code>inth()</code></td> <td>Se questo valore si trova dentro il registro di stato vuol dire che si è verificato un errore durante la scrittura</td></tr>

  <tr><td>ERR_MOD_UNKOWN</td> <td>0x05</td> <td><code>inth()</code></td> <td>Se questo valore si trova dentro il registro di stato vuol dire che la modalità precedentemente settata non è supportata dall'interfaccia</td></tr>

  <tr><td>ALL_SECT</td> <td>0x00</td> <td><code>printHD()</code></td> <td>Stampa l'intero disco</td></tr>

  <tr><td>FIRST_SECT</td> <td>0x01</td> <td><code>printHD()</code></td> <td>Stampa il primo settore del disco</td></tr>

  <tr><td>N_SECT</td> <td>0x02</td> <td><code>printHD()</code></td> <td>Stampa l'n-esimo settore del disco</td></tr>

  <tr><td>LAST_SECT</td> <td>0x04</td> <td><code>printHD()</code></td> <td>Stampa l'ultimo settore del disco</td></tr>


</table>

# INTERFACCIA
