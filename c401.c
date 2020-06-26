/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/

    // Nastavenie hodnoty korena na NULL
    *RootPtr = NULL;

}

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/

    // Ak je ukazatel na koren prazdny tak vrat FALSE, polozka nebola najdena
	if(RootPtr == NULL)
    {
        Content = NULL;
        return FALSE;
    }
	else
    {
	    // Ak sa korenovy kluc nerovna so zadanym klucom
	    if (RootPtr->Key != K)
        {
	        // ak je korenovy kluc vacsi tak pokracuj v hladany v pravom podstrome
	        if (RootPtr->Key > K)
            {
	            return BSTSearch(RootPtr->LPtr, K, Content);
            }
	        // Inak pokracuj v pravom
	        else
            {
	            return BSTSearch(RootPtr->RPtr, K, Content);
            }
        }
	    // Ak som prvok nasiel tak mi vrat jeho content a indikator TRUE
	    else
        {
            Content = &(RootPtr->BSTNodeCont);
            return TRUE;
        }

    }

}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/

    // Strom je prazdny
    if (*RootPtr == NULL)
    {
        // Vkladana polozka je koren
        if(!((*RootPtr) = (tBSTNodePtr) malloc(sizeof(struct tBSTNode))))
        {
            return;
        }
        // Nastavenie parametrov
        (*RootPtr)->Key = K;
        (*RootPtr)->BSTNodeCont = Content;
        (*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;

        return;
    }

    // Uzol s tymto klucom uz existuje tak len prepisem content
    if((*RootPtr)->Key == K)
    {
        (*RootPtr)->BSTNodeCont = Content;
        return;
    }

    // Ak je kluc zadaneho uzla vacsi tak pokracuj v hladani v lavom podstrome
    if ((*RootPtr)->Key > K)
    {
        return BSTInsert(&((*RootPtr)->LPtr), K, Content);
    }
    // Inac v pravom podsrome
    else
    {
        return BSTInsert(&((*RootPtr)->RPtr), K, Content);
    }

}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně.
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete().
**/

    // Deklaracia pomocnej premennej
    tBSTNodePtr right;

    // Koren nema pravy podstrom
	if(!(*RootPtr)->RPtr)
    {
	    // Najpravejsi uzol je koren
        right = *RootPtr;
        // priradenie kluca, dat
        PtrReplaced->Key = right->Key;
        PtrReplaced->BSTNodeCont = right->BSTNodeCont;
        PtrReplaced->LPtr = NULL;

        // Ak najpravejsi uzol ma lavy podstrom
        if (right->LPtr)
        {
            // Tak ho prirad do nahradneho laveho podstromu
            PtrReplaced->LPtr = right->LPtr;
        }

        // Uvolni pamat
        free(right);
        return;
    }

	// Pravy podstrom nema dalsi pravy podstrom
	if(!(*RootPtr)->RPtr->RPtr)
    {
	    // Najpravejsi uzol je uzol korenoveho uzlu
	    right = (*RootPtr)->RPtr;

	    // Presun hodnot z najpravejsieho uzla do nahradneho uzla
	    PtrReplaced->Key = right->Key;
	    PtrReplaced->BSTNodeCont = right->BSTNodeCont;
	    PtrReplaced->LPtr = NULL;
	    // Ak ma najpravejsi uzol lavy podstrom
	    if(right->LPtr)
        {
	        // Tak prirad lavy podstrom do nahradneho uzlu
	        PtrReplaced->LPtr = right->LPtr;
        }
	    // Uvolnenie pamata
	    free(right);
	    return;
    }

	// Zavolanei funkcie pre pravy podstrom
	ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr));

}


void BSTDelete (tBSTNodePtr *RootPtr, char K){
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/

    // Ak je root prazdny tak return
    if(*RootPtr == NULL)
    {
        return;
    }

    // Strom nie je prazdny
    if((*RootPtr) != NULL)
    {
        // AK je hladany kluc mensi jak korenovy tak pokracuj v hladnani v lavom podstrome
        if (K < (*RootPtr)->Key)
        {
            BSTDelete(&((*RootPtr)->LPtr), K);
        }
        // Inac pokracuj v hladani v pravom podstrome
        else if (K > (*RootPtr)->Key)
        {
            BSTDelete(&((*RootPtr)->RPtr), K);
        }
        // Nasiel som zhodny kluc
        else
        {
            // Ak lavy a pravy podstrom NULL
            if ((*RootPtr)->LPtr == NULL && (*RootPtr)->RPtr == NULL)
            {
                // Tak uvolni pamat a nastav ukazatel na NULL
                free(*RootPtr);
                (*RootPtr) = NULL;
            }
            // Ak lavy podstrom korenoveho uzla je NULL tak ho prirad do pomocnej premennej zrus ho a posun sa na pravy podstrom
            else if ((*RootPtr)->LPtr == NULL)
            {
                tBSTNodePtr  del = (*RootPtr);
                (*RootPtr) = (*RootPtr)->RPtr;
                // Uvolni pamat
                free(del);
            }
            // Pravy podstrom korenoveho uzla ak sa = NULL
            else if ((*RootPtr)->RPtr == NULL)
            {
                // Tak prirad do pomocnej premennej aktualnu hodnotu
                tBSTNodePtr del = (*RootPtr);
                // Posun sa na pravy podstrom
                (*RootPtr) = (*RootPtr)->LPtr;
                // Uvolni pamat
                free(del);
            }
            else
            {
                // Volaj funkciu ReplaceByRightMost pre lavy podstrom korenoveho uzla
                ReplaceByRightmost(*RootPtr, (&(*RootPtr)->LPtr));
            }
        }
    }
}

void BSTDispose (tBSTNodePtr *RootPtr) {
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/

    // Ak je korenovu uzol NULL tak return
	if(!*(RootPtr))
    {
	    return;
    }

	// Rekurzivne volanie funkcie pre lavy a pravy podstrom
	BSTDispose(&((*RootPtr)->LPtr));
	BSTDispose(&((*RootPtr)->RPtr));

	// Ukazatel na korenovy uzol nastaveny na null a uvolnenie pamata
	free(*RootPtr);
	*RootPtr = NULL;
}

/* konec c401.c */

