# Proyecto 1 DB2


## Integrantes

| Nombre y Apellidos | Código de alumno |
|-|-|
|Victor Ostolaza | 201910049 |
|Jorge Vásquez	| 201310292 |
|Jorge Rebosio | - 20182025|


## Descripcion

En este proyecto se decidió realizar la implementacion de dos estructuras de datos para el mantenimiento de estos en disco. Los elegidos fueron el **Sequential File** y el **B+Tree Unclustered**


## Objetivos

- Implementar las funciones de **Insertar**,**Buscar**,**Eliminar** para cada estructura de datos. 
- Realizar mediciones de desempeño en cada estructura y comparalos.
- Diseñar una pequeña interface de comandos para la interaccion con los datos en disco.


## Resultados Esperados

Lo que esperamos es ver el funcionamiento práctico de las estructuras vistas en clase. Asimismo resolver las diferentes causistica que cada una presenta y finalmente comprobar que el **b+tree** es mucho mas eficiente que el **Sequential File**.

# Fundamentos y técnicas de Indexación


## Sequential File

Consta de dos archivos. El primero, **data.txt**, mantiene los datos ordenados fisicamente por un campo. El segundo, **data_aux.txt**, es un espacio temporal donde se inserta en el siguiente espacio disponible y se mantiende ordenado por medio de un campo extra llamado puntero. Este archivo tiene un parametro MAX_AUX que indica la cantidad maxima de registros pueden haber en el aux_file y una vez superado ese valor se restructura la data de acuerdo a los punteros en el data_file.


### Insert

>Consiste en insertar todos los valores en el data_aux y solo cuando la cantidad de este sea igual a la cantidad máxima se restructura. La restructuración consiste en insertar nuevamente todos los resgistros usando una iteración lineal.Esta es posible gracias al **header**, el cual es un puntero que tiene como direccion el primer registro. Por otro lado,para insertar en el **data_aux** hemos considerado la logica del Free list, es decir que tenemos un header en dicho archivo que apunta a la posicion donde se debe insertar y mientras se va eliminando los registros ese puntero se actualiza como un **stack**.

### Search

>Debido a que el archivo **data.txt** se encuentra ordenado de manera fisica por un campo, se puede realizar una busqueda binaria sobre este siempre y cuando no haya un campo marcado como eliminado. Si no se llegara a encontrar el campo esperado en el data_file tras la busqueda binaria, o si el data_file contara con un registro eliminado, se procede a hacer una búsqueda secuencial.
 

### Delete

> Para eliminar en el **data.txt** debemos actualizar su puntero a -1 y como actua como una linked list, debemos hacer que el puntero del registro previo para que apunte al siguiente del que se esta eliminando. Si es que el registro que deseamos borrar se encuentra en el aux_file al eliminar un registro  se actualiza el **header_aux** con la direccion del valor a eliminar y a su vez ese valor debe tener como next el antiguo valor del **header_aux**. Esto se debe a que los eliminados en el aux_file funcionan como un stack, es decir, el último registro en ser eliminado, es el primero en ser sobreescribido.

## B+ tree Indexing Clustered 

Consta de dos archivos index.txt y data.txt. En el primer archivo guardamos los indices en forma binaria como estructura **Node** , mientras que en el segundo archivo guardamos los registros de forma binaria como estructura **Register**.En los dos archivos estas estructuras se insertan fisicamente de forma cronologica como van llegando; sin embargo, en el index existe una logica de punteros que hace que trabaje como un arbol.Asimismo las direcciones de la data se almacenan en las hojas del arbol.Ademas, el arbol cuenta con un parametro llamado ORDER el cual es el numero maximo de keys que puede tener, por lo que el numero de hijos sera ORDER+1.

     - Nota: La posicion del nodo root en el archivo index siempre sera 0.


### Insert
> Se recorre el arbol desde la raiz en busca del nodo hoja que va contener el respectivo key. Si hay espacio en el nodo hijo se agrega el key y en el children la direccion al registro creado en el datafile.Si no hay espacio se divide el nodo y actualizamos el padre. (Actualizacion recursiva).
>


### Search

> Se reccore el arbol desde la raiz en busca del nodo hoja que contiene ese key. Para ellos usamos un metodo recursivo que nos devuelve el registro si lo encuentra, caso contrario nos indica que el registro no existe.


<!-- ### Delete

> All your files and folders are presented as a tree in the file explorer. You can switch from one to another by clicking a file in the tree. -->



# Resultados Experimentales

## Sequential File

**k**=tamaño del aux file
<br>
**n**=tamaño del data file


- Busqueda

Mejor Caso  | Caso Promedio | Peor Caso
------------- | ------------- | -------------
O(1)  | O(log n) + O(k)  | O(n)

- Insercion Aux File


Mejor Caso  | Caso Promedio | Peor Caso
------------- | ------------- | -------------
O(1)  | O(1)  | O(1)


- Insercion Data File

Siempre  |
------------- |
O(n)	|


- Eliminacion

Mejor Caso  | Caso Promedio | Peor Caso
------------- | ------------- | -------------
O(1) | O(log n) + O(k)  | O(n)


- Pruebas Funcionales Insercion

| Test  | Size  |Seagate 1TB 5400RPM (s)|WD Black 1TB 7200RPM (ms) |
| :------------ |:---------------:| -----:| ------:|
| 1     | 100 | 4 | 21 |
| 2      | 500        |   52 |	412 |
| 3 | 1000        |    230 |	1479 |
| 4| 2000 | - | 6002 |
| 5 | 3000 | - | 17506|

	-Obervar el archivo tiempos.txt



## B+ tree 

**b**=ORDEN del arbol
<br>
**n**= numero de nodos

Busqueda       |     Insercion
------------- | ------------- 
 O(log b n) + O(1)   | Busqueda + reasignacion


 - Pruebas Funcionales Insercion

| Test  | Size  |WD Black 1TB 7200RPM (ms) |
| :------------ |:---------------:| -----:| ------:|
| 1     | 1000 | 52 |
| 2      | 8000 | 506 |
| 3 | 27000 | 1888 |
| 4| 64000 | 5194 |
| 5 | 125000 | 10449 |

	-Obervar el archivo tiemposbtree.txt


# Pruebas de Uso

Mostrar la interface de consola


