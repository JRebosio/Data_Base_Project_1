# Proyecto 1 DB2


## Integrantes

| Nombre y Apellidos | codigo de alumno |
|-|-|
|Victor Ostolaza | - |
|Jorge Vásquez	| 201310292 |
|Jorge Rebosio | - |


## Descripcion

En este proyecto se decidió realizar la implementacion de dos estructuras de datos para el mantenimiento de estos en disco. Los elegidos fueron el **Sequential File!** y el **B+Tree Clustered**


## Objetivos

- Implementar las funciones de **Insertar**,**Buscar**,**Eliminar** para cada estructura de datos. 
- Realizar mediciones de desempeño en cada estructura y comparalos.
- Diseñar una pequeña interface de comandos para la interaccion con los datos en disco.


## Resultados Esperados

Lo que esperamos es ver el funcionamiento práctico de las estructuras vistas en clase. Asimismo resolver las diferentes causistica que cada una presenta y finalmente comprobar que el **b+tree** es mucho mas eficiente que el **Sequential File**.

# Fundamentos y técnicas de Indexación


## Sequential File

Consta de dos archivos. El primero **data.txt**, mantiene los datos ordenados fisicamente por un campo. El segundo **data_aux.txt**, espacio temporal donde se inserta de forma adyacente como vayan llegando y se mantiende ordenado por medio de un campo extra llamado puntero. Este archivo tiene un parametro MAX_AUX que indica la cantidad maxima que registro que tendra y una vez superado ese valor se restructura la data deacuerdo a los punteros.


### Insert

>Consiste en insertar todos los valores en el data_axu y solo cuando la cantidad de este sea igual a la cantidad máxima se restructura. La restructuración consiste en insertar nuevamente todos los resgistros usando una iteración lineal.Esta es posible gracias al **header**, el cual es un puntero que tiene como direccion el primer registro. Por otro lado,para insertar en el **data_axu** hemos considerado la logica del Free list, es decir que tenemos un header en dicho archivo que apunta a la posicion donde se debe insertar y mientras se va eliminando los registros ese puntero se actualiza como un **stack**.


### Search

>Debido a que el archivo **data.txt** se encuentra ordenado de manera fisica por un campo, se puede realizar una busqueda binaria sobre este.En caso que lo encuentra lo devuelte y si no significa que puede estar en el data aux, por ello la busqueda binaria siempre retornara el registro previo al que voy a insertar y posteriormente se hace una busqueda lineal por medio de los punteros.
 

### Delete

> Para eliminar en el *data.txt* debemos actualizar su puntero a -1 y como una linked list hacer que el anterior apunte al next de este.Mientras que el *data.axu* al eliminar un registro  se actualzia el *header_axu* con la direccion del valor a eliminar y a su vez ese valor debe tener como next el antiguo valor del *header_axu*.

## B+ tree Indexing

### Insert

> All your files and folders are presented as a tree in the file explorer. You can switch from one to another by clicking a file in the tree.


### Search

> All your files and folders are presented as a tree in the file explorer. You can switch from one to another by clicking a file in the tree.


### Delete

> All your files and folders are presented as a tree in the file explorer. You can switch from one to another by clicking a file in the tree.



# Resultados Experimentales

## Sequential File

**k**=tamaño del aux file
<br>
**n**=tamaño del data file


- Busqueda

Mejor Caso  | Caso Promedio | Peor Caso
------------- | ------------- | -------------
$O(\log{}n)$  | $O(\log{}n)$ + $O(k)$  | $O(n)$

- Insercion Aux File


Mejor Caso  | Caso Promedio | Peor Caso
------------- | ------------- | -------------
$O(1)$  | $O(1)$  | $O(1)$


- Insercion Data File

Siempre  |
------------- |
$O(n)$  


- Eliminacion

Mejor Caso  | Caso Promedio | Peor Caso
------------- | ------------- | -------------
$O(\log{}n)$  | $O(\log{}n)$ + $O(k)$  | $O(n)$


- Pruebas Funcionales Insercion

| Test  | Size  | Time(seconds) |
| :------------ |:---------------:| -----:|
| 1     | 100 | 4 |
| 2      | 500        |   52 |
| 3 | 1000        |    230 |

	-Obervar el archivo tiempos.txt





# Pruebas de Uso

Mostrar la interface de consola


