/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

    // prechod polom a nastavenie vsetkych hodnot na (NULL) INICIALIZACIA
    int i = 0;
    for(; i<HTSIZE; ++i)
    {
        (*ptrht)[i] = NULL;
    }

}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key )
{
    // Hladana polozka s danym klucom
    tHTItem *item = (*ptrht)[hashCode(key)];

    int SAME = 0;
    // Prechod polom a porovnavanie klucov ci sa rovnaju
    for(; item; item = item->ptrnext)
    {
        // Ak hladany prvok najde tak vrati jeho hodnotu
        if(strcmp(key, item->key) == SAME)
        {
            return item;
        }
    }
    // Inak NULL
    return NULL;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

    // Priradenie indexu v poli (adresa) do ITEM
    tHTItem *item = htSearch(ptrht, key);
    // Vypocitanie indexu na ulozenie hodnoty
    int h = hashCode(key);

    // Ak item s tymto klucom uz existuje tak prepis len data;
    if (item)
    {
        item->data = data;
        return;
    }

    // Alokacia pamate pre novy item
    tHTItem *new_item = (tHTItem *) malloc(sizeof(tHTItem));
    // Kontrola alokacie
    if(!new_item)
    {
        return;
    }

    // Priradenie parametrov do hodnot
    new_item->key = key;
    new_item->data = data;
    new_item->ptrnext = NULL;

    // Ak existuje synonymum k danemu prvku
    item = (*ptrht)[h];
    if (item)
    {
        // Pripojime synonyma za novy prvok
        new_item->ptrnext = item;
    }
    // Na dany index ulozim novy prvok
    (*ptrht)[h] = new_item;

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key )
{
    // Priradenie hladanej polozky
    tHTItem *item = htSearch(ptrht, key);
    // Ak dana polozka existuje tak vrat data
    if(item)
    {
        return &(item->data);
    }
    // Ak nie tak NULL
    else
    {
        return NULL;
    }
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key )
{

    // Ziskanie kluca
    unsigned int h = hashCode(key);

    // Priradenie polozky s danym indexom
    tHTItem* e = (*ptrht)[h];
    tHTItem* prev = NULL;

    // Prechod cez vsetky synonyma daneho kluca, pokial nenajde spravne synonymum s klucom
    while(e != NULL)
    {
        // CMP klucov
        if(strcmp(e->key, key) == 0)
        {
            // Hladany prvok nie je na zaciatku zoznamu
            if(prev != NULL)
            {
                // Tak sa posun na dalsi prvok
                prev->ptrnext = e->ptrnext;
            }
            else
            {
                // Nastavenie nasledujuceho prvku priamo do tabulky na index dany klucom
                (*ptrht)[h] = e->ptrnext;
            }
            free(e);
            e = NULL;
        }
        else
        {
            prev = e;
            // Skok na dalsi prvok
            e = e->ptrnext;
        }
    }
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht )
{

 // Inicializacia premennych
 tHTItem *item;
 tHTItem *delete_item;

 // Prechod celym polom (hash table)
 int i = 0;
 for(; i < HTSIZE; i++)
 {
    item = (*ptrht)[i];
    // Ak ITEM nie je NULL tak vynuluj vsetko co tam je
    if(item)
    {
        delete_item = item;
        free(delete_item);
        item = item->ptrnext;
        (*ptrht)[i] = NULL;
    }
 }
}
