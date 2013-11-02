COSMOSpheres
============

<b>Bienvenido al proyecto</b>.

Antes de empezar
================

+ Aprenda C
http://www.tecnun.es/asignaturas/Informat1/AyudaInf/aprendainf/ansic/leng_c.pdf

+ Tutoriales de ZeroRobotics
https://www.zerorobotics.org/web/zero-robotics/tutorials

Github
======

Yo soy Ángel y participé en el concurso ZeroRobotics del año pasado con la EPM y
seguro que me habéis visto por allí si vais los domingos y a Compumáticas.
El año pasado nos organizábamos por Dropbox, subíamos ideas y código.
También usábamos el IDE(Integrated Development Editor) de ZeroRobotics
y el problema es que no es muy cómodo de usar, es ciertamente lento a veces(zerorobotics.org incluso se llego a caer una vez) y no facilita todo lo que podría el trabajo en grupo.
Uno de los mayores problemas con Dropbox y ZR IDE es ese mismo,
si no me creéis probad a trabajar a la vez en un mismo archivo y
esperad a que salten los errores de conflicto. Por eso estas aquí, en Github, leyendo esto.
He creado este repositorio en Github porque creo que nos ayudara a organizarnos mejor.

Github es un web-red-plataforma-(ponga aquí su pamplina)
que sirve entre otras cosas para trabajar colaborativamente en un proyecto.
Github está utiliza un software llamado Git que es un <b>VCS</b>(Version Control System),
es decir, es un programa que se encarga de mantener un registro de todas las versiones
por las que ha pasado un archivo(generalmente un archivo de código fuente) y de todos los
cambios individuales que ha sufrido el archivo. Además de eso Github tiene una <b>wiki</b> y un
<b>issue-tracker</b> que es un lugar donde se pueden escribir y hacer notar fallos, problemas y posibles
mejoras del proyecto.

Antes de continuar leyendo, por favor, abre una cuenta en Github usando el mismo correo que en ZeroRobotics y un 

Encima de este texto deberías ver los archivos de los que se compone el proyecto, si accedes a cualquiera de
ellos puedes leerlo y arriba a la izquierda encontraras tres botones:
  
  + Edit: Con este botón puedes editar el archivo. Más adelante explico un par de cosas que deberías saber de este botón
  antes de usarlo así que léelo antes de trabajar.
  
  + Raw: Te invito a que lo pulses, te devolverá el texto plano perfecto para copiar y pegar al editor de Zero Robotics.
  
  + History: Ya hemos hablado de que Github se acuerda de todas las versiones del archivo. Si pulsas aquí se te
  mostraran esas versiones, de cuando son y quien las hizo.

  + Blame: Parecido a History, te mostrara que parte del archivo fue introducido en cada versión y por tanto quien lo hizo.

Editando, editando
==================

Para permitir que varias personas puedan trabajar sin problemas en el
mismo proyecto y hasta en el mismo archivo Github implementa un
concepto llamado branch(rama), una branch es una versión alternativa
de un proyecto, empieza siendo una copia exacta y después se puede
editar ¿Pero? ¿Cuál es la utilidad de las branch's? Para explicar esto
pondré un ejemplo:

Alicia y Berto están programando las SPHERES y se reparten el trabajo:
Alicia se encarga de que la SPHERE recoja masa y suministro en la primera fase del juego
y Berto se encarga de que la SPHERE dispare el láser y modifique la trayectoria del
meteorito con su masa en la segunda fase.

Tanto Alicia como Berta están programando cosas que en su
mayor parte no se solapan para nada excepto en un lugar,
el código de main y esto sucede porque solo hay una función init() y
una función loop() y los dos necesitan poner llamadas a otras funciones ahí mismo.
Si intentan editar el archivo al mismo tiempo solo sobrevivirán los cambios del último en guardar.
El resultado es que tienen que estar vigilantes el uno del otro para no estropear main.cpp

Como las branch's hacen el mundo más feliz
==========================================

Si Alicia y Berto hubiesen usado las branch's no hubiese habido problema.
Alicia hubiese creado una branch llamada 'Recolectar suministros'
y Berto otra branch llamada 'Disparar láser' y según fuesen terminando
harían un 'merge'(que es una fusión de dos branch's) y Github haría todo
el trabajo sin molestar y en el peor de los casos, si hubiese dos versiones
contradictorias preguntaría que versión se prefiere conservar(la una, la otra o una mezcla de ambas).

Flujo de trabajo
================

1. Decidiendo en que trabajar
Si estas dispuesto a trabajar mira el issue-tracker(lista de problemas) al que puedes acceder pulsando la exclamación
a la derecha de esta página y mira entre los problemas recientes. Elige a que te vas a dedicar,
avísalo en los comentarios(cada problema tiene una sección de comentarios) y pasa al siguiente paso.
En caso de que no veas ningún problema, mira el trabajo que hay hecho y proponte algo,
después ponlo como problema con un titulo descriptivo como 'Nuestro código aun no es capaz de evitar colisiones con
el enemigo' y ponte a ello.

2. Abre una nueva branch
Si el problema que te has propuesto resolver necesita editar el código necesitaras abrir una branch para evitar
dolores de cabeza al resto: ve a la página del proyecto y busca el selector de branch(seguramente este seleccionada
la branch master que es la principal), tendrás un cuadro de texto para elegir el nombre de tu nueva branch, pon el numero
que tenga tu problema en el issue-tracker y algo descriptivo.
Quizás tu problema no requiera editar código; entonces no necesitas una branch.

3. Cook the code
Resuelve el problema. Tienes a tu disposición el editor de Github pero te recomiendo que copies el código a ZeroRobotics
y lo trabajes y pruebes en ZR. Cuando quieras guardar para dejarlo para otro momento vuelve a Github a pulsa en 'Commit'.

4. Añade tu branch al código común
Cuando creas que has solucionado el problema ve a la página del proyecto y pulsa 'Compare & pull request', comprueba los
cambios que has realizado, describe los cambios y mejoras que has introducido y pulsa 'Send pull request'. Tras eso Github
se ocupará de resolver los conflictos y el código de la branch master incluirá tus mejoras.


English Version
===============
A repository created to help the alliance Sui Generis - EPM Miguel de Guzmán - Casual Developers

Useful links
============
Game manual
API reference: http://www.zerorobotics.org/documents/10429/374963/ZR_user_API.pdf
Zero Robotics tutorials

GitHub
======
As you should already know, we have chosen to write code through this page rather than using the official 
IDE provided by Zero Robotics. You may ask, why so? To answer your curiosity, I will simply invite you to 
try and edit a snippet of code in ZR IDE while one of your fellow companions is already working on it. Can
you see the multiple errors popping out of nowhere? Yes? Well, that's your reason to change to GitHub.

<b>What is Github?</b>
GitHub is a platform developed to help open source programmers to coordinate their efforts and write code
together. To do that it employs Git, a VCS (Version Control System) software which keeps track of the changes
made to the repository by every developer collaborating in a proyect.
