
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "tp.h"
#include "code.h"
#include "tp_y.h"

extern char* strdup(const char *);
extern ScopeP env;
FILE* output;

/*
 * GENERATION DE CODE
 *
 */

/* Fonctions basiques servant à la génération de code
 * pour la machine virtuelle
 */
void NOP() {fprintf(output, "NOP\n");}

/*ERR str, START, STOP*/
void PUSHI(int i) {fprintf(output, "PUSHI %d\n", i); printf("PUSHHHHHHH---");}
void PUSHS(char *c) {fprintf(output, "PUSHS %s\n", c);}
void PUSHG(int a) {fprintf(output, "PUSHG %d\n", a);}
void PUSHL(int a) {fprintf(output, "PUSHL %d\n", a);}

/*PUSHSP, PUSHFP*/
void STOREL(int x) {fprintf(output, "STOREL %d\n", x);}
void STOREG(int x) {fprintf(output, "STOREG %d\n", x);}
void PUSHN(int x) {fprintf(output, "PUSHN %d\n", x);}
void POPN(int x) {fprintf(output, "POPN %d\n", x);}
void DUPN(int x) {fprintf(output, "DUPN %d\n", x);}
void SWAP() {fprintf(output, "SWAP\n");}
void EQUAL() {fprintf(output, "EQUAL\n");}
void NOT() {fprintf(output, "NOT\n");}
void JUMP(char* label) {fprintf(output, "JUMP %s\n", label);}
void JZ(char* label) {fprintf(output, "JZ %s\n", label);}
void PUSHA(char* a) {fprintf(output, "PUSHA %s\n", a);}
void CALL() {fprintf(output, "CALL\n");}
void CRETURN() {fprintf(output, "RETURN\n");}
void CADD() {fprintf(output, "ADD\n");}
void CMUL() {fprintf(output, "MUL\n");}
void CSUB() {fprintf(output, "SUB\n");}
void CDIV() {fprintf(output, "DIV\n");}
void CINF() {fprintf(output, "INF\n");}
void CINFEQ() {fprintf(output, "INFEQ\n");}
void CSUP() {fprintf(output, "SUP\n");}
void CSUPEQ() {fprintf(output, "SUPEQ\n");}
void WRITEI() {fprintf(output, "WRITEI\n");}

/*STR*/
void WRITES() {fprintf(output, "WRITES\n");}
void CCONCAT() {fprintf(output, "CONCAT\n");}
void STORE(int x) {fprintf(output, "STORE %d\n", x);}
void LOAD(int x) {fprintf(output, "LOAD %d\n", x);}
void ALLOC(int x) {fprintf(output, "ALLOC %d\n", x);}

void PUSHG_addr(char* c) {
  /*int a = getAddr(c);
    if (a < 0) {fprintf(output, "PUSHG addr(%s)(%d)\n", c, a);}
    else {PUSHG(a);}*/
}

void PUSHL_addr(char* c) {
  /*int a = getAddr(c);
    if (a < 0) {fprintf(output, "PUSHL addr(%s)(%d)\n", c, a);}
    else {PUSHL(a);}*/}

void STOREG_addr(char* c) {
  /*int a = getAddr(c);
    if (a < 0) {fprintf(output, "STOREG addr(%s)(%d)\n", c, a);}
    else{STOREG(a);}*/}

void LABEL(char* c) {
    fprintf(output, "%s : \t", c);}

void NEWLABEL(char* c) {
    LABEL(c);
    NOP();}

/*void NEWMETHODLABEL(char* meth, char* cla) {
    fprintf(output, "%s_%s: ", cla, meth);
    NOP();
}
*/


/* Retourne l'adresse d'une variable contenue dans l'environnement
 * de variables. 
 * Sera utile pour faire PUSHG Id.adresse() par exemple.
 */
int adresse(char *id) 
{

/*
    On va parcourir l'environnement de variables et prendre la première qui correspond,
    et return son adresse (créée arbitrairement lors de sa déclaration.
*/


/* Dans l'environnement de variables

    Il faut :
    Portée : Globale, Locale, Param
    Type : Attribut
    Offset
    Valeur

    Il faut savoir gérer l'appel de fonction (x.f(), passer x en paramètres ?)

    Nombre d'attributs dans une classe afin de faire ALLOC 3 par ex dans le cas d'un new, alloc de mémoire

*/



/*
    bubble_ptr bub = stack.top_bubble;
    while(bub) {
        SymboleP sy = bub->symboles;
        while (sy) {
            if (!strcmp(name,sy->var->name)) {
                return sy->addr;
            }
            sy = sy->next;
        }
        bub = bub->next;
    }
    fprintf(out, "couldn't find %s in :", name);
    showEnv();
    return -1;

    */
  return 0;
}

VarDeclP getVarDeclFromName(char *nom)
{
	/*ScopeP env : env de variables globales*/
	LVarDeclP temp = env->env;
	while(temp != NULL)
    {
    	if(strcmp(nom, temp->var->nom) == 0)
    	{
    		return temp->var;
    	}
    	temp = temp->next;
    }
    printf("Erreur environnement : variable %s introuvable.\n", nom);
    return NULL;
}


/*retourne le décalage d'une variable par rapport
 *au fond de pile de sa classe*/
int getOffset(ClasseP classe, char *idNom)
{
	/*TODO : classe extends*/
	int offset = 0;

    LVarDeclP temp = classe->lchamps;
    printf("PRINT VAR DECL :: \n");
    printVarDecl(temp);
	printf("Var de la classe %s : \n", classe->nom);

    while(temp != NULL)
	{
		printf(">%s\n", temp->var->nom);
        if(strcmp(temp->var->nom, idNom) == 0)
        {
        	printf("Offset = %d\n",offset);
            return offset;
        }
        else
        {
            offset += 1;		/* TODO bizarre : renvoie 0 pour la dernière variable déclarée.*/
        }						/*ça veut dire que c'est la variable qui est à 0 du fond de pile. C'est pas le cas si ?*/
        temp = temp->next;	/*est égal à NULL s'il n'existe qu'un élément dans la liste*/
    }	
    printf("Erreur offset.\n");
	return -1;
}

/*
 * liste d'instructions
 */
void codeLInstr(TreeP tree)
{
    if(tree->nbChildren == 2)
    {
        codeInstr(getChild(tree, 0));
        codeLInstr(getChild(tree, 1));

    }
    else
    {
        codeInstr(tree);  
    }
}

/* Renvoie une étiquette toujours différente, en fonction
 * du type indiqué en paramètre.
 * Permet d'obtenir if3 ou else10 par exemple.
 */
char *makeLabel(char *type)
{

    static char buf[30];
    static int cpt;

    sprintf(buf, "%s%d", type, cpt++);        /*incrementer l'étiquette statique*/

    return strdup(buf);
}

/*Génère le code d'un bloc ITE*/
void codeITE(TreeP tree)
{
    /*Création des étiquettes*/
    char *labelElse = makeLabel("else");
    char *labelEndIf = makeLabel("endif");

    /*ajout de la condition du if*/
    codeExpr(getChild(tree, 0));

    /*JZ vers le bloc else si la condition ci-dessus est fausse*/
    fprintf(output, "JZ %s\n", labelElse);

    /*Corps du if*/
    codeInstr(getChild(tree, 1));

    /*Si on effectue le code du corps du if, on saute le corps du when*/
    fprintf(output, "JUMP %s\n", labelEndIf);

    /*Etiquette du else*/
    fprintf(output, "%s:", labelElse);
 
    /*Corps du else optionnel*/
    TreeP childElse = getChild(tree, 2);
    if(childElse)
    {
        codeInstr(getChild(tree, 2));
    }
    else
    {
       fprintf(output, "\n");
    }

    /*Etiquette de sortie de la boucle*/
    fprintf(output, "%s: NOP\n", labelEndIf);
}

/* 
 * Génère le code d'une instruction.
 */
void codeInstr(TreeP tree)
{
    switch (tree->op) {
    case YEXPR:                     		/* Expr;*/

        codeExpr(getChild(tree, 0));   
        printf("YEXPR\n");
       /* POPN(1); a quoi sert-il ?*/
        break;

    /*atteignable à travers Bloc ;*/
    /*Generation du code d'un bloc*/
    case YCONT:                 			/*LDeclChamp IS LInstr */

        fprintf(output, "--########Bloc de type LDeclChamp IS LInstr--\n");
        /*LdeclChamp = list variables locales*/
        codeLDeclChamp(getChild(tree, 0));
        codeLInstr(getChild(tree, 1));
        printf("YCONT\n");
        break;

    /*atteignable à travers Bloc ;*/
    /*Generation du code d'un bloc*/
    case LINSTR:        
        codeLInstr(tree);
        printf("LINSTR\n");
        break;
    
    case YRETURN:                            /*modifier tp.y et tp.h pour y ajouter YRETURN!!!!!!!!!!!!!!!!!!!!!!!!!	$$ = makeTree(YRETURN, 0); */
        CRETURN();
        printf("YRETURN\n");
        break;
    
    case EAFF:
        codeAff(tree);
        printf("EAFF\n");
        break;

    case YITE:
        codeITE(tree);
        break;

    default: 
        fprintf(output, "Cas Instruction inconnu : %d! son contenu est : %s\n", tree->op, tree->u.str);
        break;
    }   

}

/*Code du constructeur*/ 
void codeConstructeur(TreeP tree)     					/*Expr: NEWC TypeC '(' LExprOpt ')'*/
{

	fprintf(output, "------------------ := Instanciation new %s():\n", getChild(tree, 0)->u.str);

	/*On récupère la classe à instancier*/
	/*ClasseP classe = getClasse(getChild(tree, 0)->u.str);
    
	TreeP LExprOpt = getChild(expr, 1);

	int nombreExprOpt = classe.getTailleParam();

  	Ce que va faire le constructeur : 
	for (i = nb de param)
            DUPN 1
            codeLExprOpt()
            STORE(i)
    */

    /* 
    TreeP class_id = getChild(expr, 0);
            TreeP rexprl = getChild(expr, 1);
            ClassP class = figureClass(class_id->u.str);
           
            int nbfields = sizeOfType(class);
            ALLOCATE(nbfields);
            
            TreeP next_expr = rexprl;
            
            for (int i=0;i<nbfields && next_expr;i++) {

                DUPN(1);
                codeExpr(getChild(next_expr, 0), 
                next_expr = getChild(next_expr, 1);
                STORE(i);
            }
            
        break; 
        */
}

/*Code des expressions*/
void codeExpr(TreeP tree)
{
    switch(tree->op) {

        case Cste:
            PUSHI(tree->u.val);
            printf("PUSHI\n");

        break;

        case Chaine:
        case Classname:
            PUSHS(tree->u.str);
            printf("PUSHS\n");
        break;
    
        case SELEXPR:
            codeSelec(tree);
            printf("SELEXPR\n"); 
            break;

        case Id:
            /*get adresse de la variable Id
            Comme ça :
            if (in_method) PUSHL_addr(tree->u.str); 
            else PUSHG_addr(tree->u.str)

            Il faut comprendre pourquoi on pushl dans une méthode et on pushg autre part ?
            pushl : push la valeur vers le pointeur fp (voir poly genCode)

            fp est le pointeur de la pile après qu'on ait fait START (affecte sp à fp)

            */

            /*TODO !!!!!!!!!!!!*/
            fprintf(output, "PUSHG %s.adresse()\n", tree->u.str);

            break;

        /*instanciation/constructeur*/
        case EINST: 
            printf("Instanciation : EINST\n"); 

            codeConstructeur(tree);
            /*TODO*/     
            break;

        /*ExprOperateur*/
        case ADD :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            CADD();
            printf("ADD\n");
            break;

        case SUB :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            CSUB();
            printf("SUB\n");
            break;

        /*on traite seulement le cas du moins unaire, le plus unaire reviendrait à ne rien faire*/
        case USUB :
            printf("unarySUB\n");
            /*on fait 0 moins l'expression */
            PUSHI(0);
            codeExpr(getChild(tree, 0));
            CSUB();
            break;

        case MUL :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            CMUL();
            printf("MUL\n");
            break;
        case DIV :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            printf("DIV\n");
            CDIV();
            break;
        case CONCAT:
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            CCONCAT();
            printf("CONCAT\n");
            break;

        /*Relop*/        
        case NE:
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            EQUAL();
            NOT();
            printf("NE\n");
            break;
        case EQ :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            EQUAL();
            printf("EQ\n");
            break;
 
        case INF :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            CINF();
            printf("INF\n");
            break;

        case INFE :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            CINFEQ();
            printf("INFE\n");
            break;

        case SUP :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            CSUP();
            printf("SUB\n");
            break;

        case SUPE :
            codeExpr(getChild(tree, 0));
            codeExpr(getChild(tree, 1));
            CSUPEQ();
            printf("SUPE\n");
            break;
        
        case EENVOI:
            codeEnvoi(tree);
            printf("EENVOI\n");
            break;

        default: 
            if(tree->nbChildren != 0)
            {
                codeExpr(getChild(tree, 0));
            }
            printf("#ERREUR : Expression inconnue : %d!\n", tree->op);
            break;
        }
}

/*Génère le code des fonctions print() et println()*/
bool codePrint(TreeP expr, TreeP methodeC)
{
	if(expr->op == Cste)
	{
		fprintf(output, "\n--%s affiche la constante suivante :\n", getChild(methodeC,0)->u.str);
		codeExpr(expr);
		if (!strcmp(getChild(methodeC,0)->u.str,"print")) {				/*écriture simple de la chaine en début de pile*/
            WRITES();
            return TRUE;
        }
        else if (!strcmp(getChild(methodeC,0)->u.str,"println")) {		/*écriture de la chaine et saut de ligne*/
            WRITES();
            PUSHS("\"\\n\"");											/*équivaut à PUSHS "\n" */
            WRITES();
            return TRUE;
        }
	}
	else if(expr->op == Chaine)
	{
		fprintf(output, "\n--%s affiche la chaine suivante :\n", getChild(methodeC,0)->u.str);
		codeExpr(expr);
		if (!strcmp(getChild(methodeC,0)->u.str,"print")) {				/*écriture simple de la chaine en début de pile*/
            WRITES();
            return TRUE;						
        }
        else if (!strcmp(getChild(methodeC,0)->u.str,"println")) {		/*écriture de la chaine et saut de ligne*/
            WRITES();
            PUSHS("\"\\n\"");											/*équivaut à PUSHS "\n" */
            WRITES();
            return TRUE;
        }
	}
	return FALSE;
}

/*Génère le code d'un envoi*/ 
void codeEnvoi(TreeP tree)					/*Envoi: Expr '.' MethodeC */
{
	/*TODO*/
    fprintf(output, "--IL Y AURA UN ENVOI vers %s\n", getChild(tree, 0)->u.str);
   	TreeP Expr = getChild(tree, 0);										
	TreeP MethodeC = getChild(tree, 1);

	/*Si la méthode est print ou println, on génère son code*/
	bool printOK = FALSE;
	printOK = codePrint(Expr, MethodeC);

	/*Si la méthode n'est pas un print ou println*/
	if(printOK == FALSE)
	{
		/*On exécuter le code de l'expression*/
		/*?TODO??codeExpr(Expr); ????????????????????????????????????????????????????*/
		
		/*On récupère la classe correspondant à l'expression*/
 		VarDeclP tempExpr = getVarDeclFromName(Expr->u.str);
    	char *type = tempExpr->type->nom;
    	ClasseP classeEnvoi = getClassePointer(type);

    	/*On récupère la structure correspondant à MethodeC*/
    	MethodeP tempMethode = getMethodeFromName(classeEnvoi, getChild(MethodeC,0)->u.str);
    	
        
    	if(tempMethode)
    	{
    		LVarDeclP tempParam = tempMethode->lparametres;
    		printf("\n\n--Envoi : méthode %s de la class %s\n", tempMethode->nom, classeEnvoi->nom);

			int nbParam = 0; 
			while(tempParam != NULL) 
			{
				PUSHG(nbParam);  /*empiler l'argument ?????????????????????????????????????????????????????*/ 
				nbParam++;
				tempParam = tempParam->next;
			}

			/*Appel (statique...) de la méthode*/
			char *adresseMethode = tempMethode->nom;
			fprintf(output, "\n--Appel de la methode %s.\n", adresseMethode);
			PUSHA(adresseMethode);
	    	CALL();

	    	/*On dépile le nombre de paramètres empilés*/
			POPN(nbParam);
    	}
    


	}

	/*Créer des méthodes avec des étiquettes uniques ?*/
	/*Question : comment marche l'empilement de l'envoi ?
	est-ce qu'on empile d'abord la partie gauche puis la partie droite ?*/
}

/*Appel de méthode : 
	class C is f(){}
	
    {var monC : C = new
	   monC.f
    }

	START
	PUSHN 1 -- allocation de ta variable monC 
	PUSHG 0
	PUSHA f1 (ça c'est en statique, dynamique plus compliqué que ça mais en principe c'est ça)
	
	CALL ??? why what
	STOP

	--fonctions 
	f1: NOP            -methode f   
		corpsf1
		RETURN
    f2: skdfsd
        RETURN

 
    f546456: WRITES   -tostring
            RETURN
*/

MethodeP getMethodeFromName(ClasseP classe, char *nom)
{
	LMethodeP temp = classe->lmethodes;
	while(temp != NULL)
    {
    	if(strcmp(nom, temp->methode->nom) == 0)
    	{
    		return temp->methode;
    	}
    	temp = temp->next;
    }
    printf("Erreur methode : methode %s  introuvable.\n", nom);
    return NULL;
}

/*Génère le code d'une sélection*/
void codeSelec(TreeP tree)		                 /*Selection: Expr '.' Id*/		/*Pb du id dans la production Selection?*/
{
	/*TODO*/
    /*sélections comment faire ? mettre le x de x.t en paramètres de f ? prof a dit ça*/
    fprintf(output, "--IL Y AURA UNE SELECTION ICI;\n");

    /*Voir le code de paire.entiers dans Interp*/
    /*
    Voir le type de retour de l'expression
    puis chercher le id dans la classe correspondante
    */

    /*cas où Expr est un ident*/
    TreeP Expr = getChild(tree,0);
    TreeP Ident = getChild(tree,1);

    if(Expr->op == Id)
    {
    	VarDeclP temp = getVarDeclFromName(Expr->u.str);
    	char *type = temp->type->nom;
    	ClasseP classeType = getClassePointer(type);

    					/*???if (in_method) PUSHL_addr(expr->u.str); else*/ 
    					/*???PUSHG_addr(expr->u.str);*/
    	fprintf(output,"PUSHG %s.adresse()\n", Expr->u.str);
            
    	int offset = getOffset(classeType,Ident->u.str);
    	LOAD(offset);
    }
	/*
 	if (expr->op == IDVAR) {
            ClassP t = figureClass(getSymbole(expr->u.str)->var->type);
            if (t) fprintf(out, "--type should be %s\n", t->name);
            else return;
            margin(d); 

            if (in_method) PUSHL_addr(expr->u.str); else PUSHG_addr(expr->u.str);
            
            int x = getOffset(t,id->u.str);
            margin(d); LOAD(x);
    */
}


/*Code d'une affectation x := y*/
void codeAff(TreeP tree)
{
    /*membre gauche de l'affectation gauche := droit*/
    TreeP gauche = getChild(tree, 0);

    /*membre droit de l'affectation*/
    TreeP droit = getChild(tree, 1);

    /*pour l'instant on gère les affectations de constante et d'id*/

    if( getChild(tree, 0)->op == Id)
    {
        if(getChild(tree, 1)->op == Cste)
        {
			fprintf(output, "Affectation : constante %d\n", droit->u.val);
            codeExpr(droit);
            /*STOREG(gauche.getAdresse());*/
            fprintf(output,"STOREG %s.adresse()\n", gauche->u.str);
		}
        else
        {
            fprintf(output, "Affectation : ident %s\n", droit->u.str);
            codeExpr(droit);
            /*STOREG(gauche.getAdresse());*/
            fprintf(output,"STOREG %s.adresse()\n", gauche->u.str);
        }
    }

/*	if (gauche->op = id et droit->op = eaff)
		on fait constructeur(droit)
		on fait storeg gauche.getadresse()

	if(gauche->op = id et droit->op != eaff)
		on pushl ou pushg id
		on codeExpr(droit)	

	if gauche->op = selection 
		if(gauche.filsgauche = est une classe)	
*/
/*	if (selectorid->op == IDVAR) {

        if (expr->op == EALLOC) {

            codeExpr(expr);
            
            STOREG_addr(selectorid->u.str);
        }
        else {
            
            if (in_method) PUSHL_addr(selectorid->u.str); else PUSHG_addr(selectorid->u.str);
            codeExpr(expr);
             il faut générer l'adresse de la variable locale ou du champ d'objet 
            
             STORE(offset);
             x = 0 si variable locale, x = ? offset du champ de classe à déterminer 
        }
    }
    else if (selectorid->op == EPOINT) {
        TreeP var = getChild(selectorid, 0);
        TreeP field = getChild(selectorid, 1);
        if (var->op == IDVAR) {
            
            if (in_method) PUSHL_addr(var->u.str); else PUSHG_addr(var->u.str);		JE TROUVE CA BIZARRE
            codeExpr(expr, depth+1);
            char* t = getSymbole(var->u.str)->var->type;
            ClassP class = figureClass(t);
            offset = getOffset(class,field->u.str);
           
            STORE(offset);
        }
        else {
        }
    }
*/
}

/*Code d'un bloc objet situé dans les objets isolés et dans les classes*/
void codeBlocObj(TreeP tree)        /*BlocObj: '{' LDeclChampMethodeOpt '}' */
{

    if(tree->nbChildren == 2)
    {
    	/*ptet qu'on ne peut pas appeler codeBlocObj ???*/
        codeBlocObj(getChild(tree, 1));                     /*LDeclChampMethode: LDeclChampMethode DeclChampMethode*/
        codeDeclChampMethode(getChild(tree, 0));            /* LDeclChampMethode: DeclChampMethode*/
    }
    else if(tree->nbChildren == 1)
    {
		codeDeclChampMethode(getChild(tree, 0));
    }
   

}

void codeDeclChampMethode(TreeP tree)
{
    if(tree->op == YDECLC) /*DeclChamp*/
    {
        printf("DECL CHAMP\n");
        codeDeclChamp(tree);
    }

    else if(tree->op == DMETHODE) /*DeclMethode pas besoin de gérer ce cas, 
    								vu qu'on met les méthodes dans des structures*/
    {
        printf("DECL METHODE qu'on appelera à partir de classe\n");
       /* MethodeP methode = getMethodeFromName(tree->u.str);  
       	codeDeclMethode(methode);
*/    }

}


/*Liste de déclarations champ*/
void codeLDeclChamp(TreeP tree)		
{

    /*la liste marche bien, 
    elle reconnait toutes les déclarations normalement.
    Testé avec des fprintf*/
    if(tree->nbChildren == 2)				/*LDeclChamp: DeclChamp LDeclChamp */
    {
        codeDeclChamp(getChild(tree, 0));
        codeLDeclChamp(getChild(tree, 1));
    }
    else 									/*LDecleChamp: DeclChamp*/
    {	
        codeDeclChamp(tree);  
    }
}



void codeDeclChamp(TreeP tree)		        /*DeclChamp: VAR Id ':' TypeC ValVar ';'*/
{                                           /*ValVar: AFF Expr*/

	/*il faut STROREG id.adresse à la fin ?*/

    /*TODO : refaire mais avec la méthode makeVarDecl!!!!!!!!!!!!!!!!!!!*/

    fprintf(output, "----BIENVENUE DANS LE DeclChamp.\n");

    fprintf(output,"--Var %s : ", tree->u.var->nom);
        /*codeExpr(getChild(tree, 1));  correspond au typeC*/

   /* fprintf(output, "%s", getChild(tree, 1)->u.str);*/    /*comment gérer le type d'une expression en terme de gén de code ?*/

  /*  TreeP valVar = getChild(tree, 2);
    if (valVar)
    {
        codeExpr(valVar);
    }
    fprintf(output, "\n");
    */

    /* VariableP var = tree->u.lvar;

    ajoutVariable(var) à l'environnement : initialiser l'adresse de la variable,
    afin de pouvoir la STOREG dans le futur
    
    déclaration ! comment ?
    
    histoire de constructeur ?

    sinon pour une alloc normale on a : 

    alloc 1
    dupn 1
    codeExpr(Valvar);
    store(0)
    */
}

/*Methodes d'une classe*/
void codeDeclMethode(MethodeP methode)	/*DeclMethode: OverrideOpt DEF Id '(' LParamOpt ')' ':' TypeC AFF Expr*/
{									/*DeclMethode: OverrideOpt DEF Id '(' LParamOpt ')' TypeCOpt IS Bloc */

	fprintf(output,"--Declaration de la methode %s.\n", methode->nom);

	fprintf(output, "%s: \t", methode->nom);
	/*codeBlocObj(tree);*/

    /*en principe : on compte le nombre de parametres avec un while param->next

    ensuite on empile en gardant en mémoire la taille de la pile au moment où tu rentres dans la fonction.

    
    rang param?????

    if les méthodes sont un bloc => codeBlocObj if(tree->op == DMETHODE)
    else if méthode sont une expr => codeExpr               DMETHODEL


    PUSHL(0 - (nbPAram+1))
    SWAP() pk ? lol
    STORE(0) 

    RETURN
    
    if methode->next != null ; codeMethode

    */
}


/*Génère le code d'une classe*/
void codeClasse(ClasseP classe)
{
    
    fprintf(output, "--Declaration d'une classe :\n");

    LMethodeP liste = classe->lmethodes;

    while(liste != NULL)
    {
    	codeDeclMethode(liste->methode);
    	liste = liste->next;
    }
    /*

    fprintf(output, "-- declaring class %s:\n", classe->name);
     generate code constructor
    
     methods
    generateCodeMethod(classe->methods,class);

    fprintf(output, "-- end of class %s\n", classe->name);
    if (class->next)
    generateCodeClass(class->next);*/ 
}


void codeLClasse(TreeP tree)
{
    fprintf(output, "--Generation de code d'une liste de classe...\n");

    /*
    if(tree->nbChildren == 2)				
    {
        codeDeclChamp(getChild(tree, 0));
        codeLDeclChamp(getChild(tree, 1));
    }
    else 									
    {	
        codeDeclChamp(tree);  
    }
	*/

/*
    LClasseP liste = classe->lmethodes;

    while(liste != NULL)
    {
    	codeDeclMethode(liste->methode);
    	liste = liste->next;
    }
*/

    /*Parcours l'environnement de classe
        
    temp = getPremièreClasseDeLaListe ?

    while(temp->next != NULL)
    {
        codeClasse(temp);
        temp = temp->next;
    }

    */
}


void genCode(TreeP LClass, TreeP Bloc /*, Environnement *env, ClassesEnv* cenv, Adresse **addr*/)
{

    output = fopen("bailtest", "a+");

    if (output != NULL) {

        fprintf(output, "START\n");
        /*codeLClasse(LClass);*/
        codeInstr(Bloc);
        fprintf(output, "STOP\n");

        fclose(output);
    }


}