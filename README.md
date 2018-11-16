# Compilation Project

## Introduction

*English*  

This is a project aiming to compile an [object oriented language](https://github.com/minh-n/CompilationProject/blob/master/test/ex1.txt) using a lexer, an abstract syntax tree and a code generator/interpretor. This project has been made with a team of 4 over the course of two months for the Compilation class by Mr Frederic VOISIN. 


*Français*

Voici le répertoire du projet de compilation d'ET4, dont le but était de compiler un [langage orienté objet](https://github.com/minh-n/CompilationProject/blob/master/test/ex1.txt) utilisant un analyseur lexical, un arbre syntaxique abstrait et un générateur/interpréteur de code. Nous avons travaillé sur ce projet pendant une durée de deux mois et dans une équipe de 4 personnes. Ce projet fait parti du cours de compilation de M. Frédéric VOISIN.

État actuel du projet : le programme est capable de reconnaître entièrement un programme du langage grâce à des expressions régulières et de générer un code assembleur simple, par exemple le fichier ```simple.txt```. Le code généré, lorsque passé dans l'interpréteur, effectue la déclaration d'objets et l'affichage de leurs variables. 


## Annexes

### Annexe A : Liens dynamiques

Au début du programme, on alloue une table virtuelle avec des appels de méthodes dedans (fichier dynlink) :

```
-- initialiser les tables virtuelles
init:
    ALLOC 2 -- classe A (PREMIERE PARTIE DE LA TABLE VIRTUELLE)
    DUPN 1
    PUSHA m1
    STORE 0
    DUPN 1
    PUSHA m2
    STORE 1
```


    ALLOC 2 -- classe B  (DEUXIEME PARTIE DE LA TABLE VIRTUELLE)
    DUPN 1
    PUSHA m3 -- redefinit m1
    STORE 0
    DUPN 1
    PUSHA m2 -- pas de redefinition
    STORE 1

On a une classe A qui peut appeler les méthodes m1 et m2 (qu'on définira après l'initiation de cette table virtuelle) 
et une classe B qui peut appeler les méthodes m3 et m2.


On déclare ensuite les méthodes m1, m2 et m3 qui sont de simples println (PUSHS ...).

On déclare ensuite deux méthodes call1 et call2 qui serviront à appeler m1, m2 ou m3.
```
call1:  PUSHL -1    -- le recepteur
    DUPN 1
    LOAD 0 -- charger TV
    LOAD 0 -- charger @methode1
    CALL
    RETURN
```

call1 va prendre la table virtuelle de l'objet (donc soit la première partie, qui correspond à A, soit la deuxième) et appeler la méthode.

-----------------------------------------------

Dans le main, on va allouer les variables pour A et B.
```
-- allouer un objet de classe A
    ALLOC 1 -- a
    DUPN 1
    PUSHG 0
    STORE 0
```
Dans ce tas de l'objet a nouvellement créé, on va PUSHG l'adresse de la PREMIERE PARTIE de la table virtuelle (correspond au premier ALLOC 2 juste après le init).

Ensuite dans le main, on va pouvoir appeler les méthodes propres à A (m1 et m2). Par exemple, a.m1 sera :
```
    -- invoquer la premiere methode sur a: m1
    PUSHL 0     --on prend la TV de a qui correspond à la première partie de la TV (classe A)
    PUSHA call1  --on appelle call1 sur cette première partie de la TV, qui ne contient donc que les méthodes de la classe A
    CALL
    POPN 1
```

### Annexe 2 : Exemple de programme très simple pouvant être interprété par le programme

```

class Point(xc: Integer, yc: Integer) {
     this.x:= xc; this.y:= yc; 
  } 

  is 

  {

  var x: Integer;
  var y : Integer := 5;

  var name: String := "?";

  def getx() : Integer := this.x
  def gety() : Integer := this.y

  def setName(s: String) is { this.name := s; }

  def getName() : String := this.name 


}
{ 

  var p1: Point := new Point(98, 99);
  var p2: Point := new Point(2048, 4095);
  is
  "Bonjour monde".println();

  p2.setName("glop");

}
```


    CALL
