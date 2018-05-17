# Red_sensores
Tecnológico de Costa Rica
Escuela de Ingeniería en Electrónica
Programa de Licenciatura en Ingeniería en Electrónica
Curso: EL-4314 Arquitectura de Computadores I
Especificación Proyecto 2

2. Descripción del protocolo a implementar
El presente protocolo se propone y se describe de forma detallada en la tesis de maestría
adjunta, a continuación, se presenta un resumen con el objetivo de facilitar su
implementación.

La conexión de los nodos en una red de sensores inalámbrica es aproximadamente una
estrella extendida, en la que la conexión se encuentra condicionada al alcance de la etapa
de radiofrecuencia de los nodos; como se muestra en la figura 1. Partiendo de esto, se
propone una red organizada jerárquicamente en niveles, en la que se pueden definir como
nodos de nivel 1 todos los nodos que se encuentren en el alcance de la estación base
(zona gris), los nodos de nivel 2 serán los que se encuentren en el alcance de algún nodo
de nivel uno y no de uno de nivel cero (zona amarilla) y así sucesivamente.

Es necesario que los mensajes transmitidos por un nodo de nivel N pasen en el mejor de
los casos por N −1 saltos antes de llegar a un sumidero.


Figura 1: ilustración de la conectividad de una red de sensores

Se asume que el medio de transmisión de los nodos es compartido, (por ejemplo: todos
los nodos transmiten en la misma frecuencia) por lo que es posible que si 2 nodos
intentan transmitir al mismo tiempo los datos se pueden corromper. Para evitar este
problema es necesario implementar un protocolo de control de acceso al medio.
Además, es necesario implementar un protocolo de enrutamiento o de red que permita
la selección de la ruta que deben tomar los paquetes, y para reducir el tráfico en la red y
con esto el consumo de potencia y la latencia de las tramas se propone un algoritmo de
agrupamiento y compresión de los datos. Dado que se implementan varias capas del
modelo de referencia OSI dentro del protocolo, a este se le llama un protocolo multicapa.

El algoritmo propuesto se detalla en la figura 2; se puede entender como una serie de
estados sobre los que cada uno de los nodos pasa, y que resultarán en la correcta
transmisión y recepción de los datos. El protocolo se puede entender como dos algoritmos
de corren de manera concurrente, el primero es el algoritmo de selección de ruta de datos
mezclados (ADM) y el segundo es el de agrupamiento y mezcla de datos (AMD).


Figura 2: Diagrama de flujo del algoritmo multicapa propuesto

Selección de ruta de datos mezclados (ADM)

Como su nombre lo indica, este es el algoritmo con el que se enviarán los datos que ya
han sido mezclados hacia los sumideros de la red. Se propone que cada uno de los nodos
pase a través de una serie de estados que contemplan un estado de hibernación para
reducir el consumo de energía en el módulo de comunicación; Además se procura que el
protocolo tenga la capacidad de reconfigurar la arquitectura de la red en proceso de
instalación, falla de nodos y/o ampliación del sistema sin necesidad de intervención
externa (AD-HOC).

A continuación, se explica la función de cada uno de los estados del protocolo desde el
punto de vista del algoritmo de selección de ruta de datos mezclados ADM.

Descubrimiento de nivel de pertenencia: En esta etapa el nodo descubre su nivel de
pertenencia en la red; esto lo hace basándose en la definición de nivel que se proporcionó
anteriormente: Se definen como nodos de nivel uno todos los nodos que se encuentren en
el alcance de la estación base, los nodos de nivel dos serán los que se encuentren en
el alcance de algún nodo de nivel uno y así sucesivamente. De esta definición se puede
entender que la pertenencia a un nivel u otro se encuentra en función del radio de alcance
del transmisor del nodo de nivel inmediatamente inferior y la posición geográfica de los
nodos. En este estado se escucha el medio durante un tiempo suficiente (Estos tiempos
se especificarán con más detalle posteriormente, en este caso será de dos veces el
tiempo de hibernación, también llamado “T”) para detectar las tramas PT (Petición de
trama) de todos los nodos cercanos y comparar su nivel en la red. Una vez que se tienen
los niveles de todos los nodos cercanos se selecciona el menor nivel escuchado y se
sabe que el nodo que escucha se encontrará en el nivel menor escuchado más uno. Al
finalizar este proceso se envía una trama de alarma especial que indica que el nodo entró
en operación. En caso de que el nodo escuche por el tiempo predeterminado para este
periodo sin oír una sola trama de PT, el nodo pasará al estado de hibernación para
ahorrar energía por un periodo predeterminado y volverá nuevamente al estado de
descubrimiento de nivel de pertenencia, repitiendo el ciclo hasta que se tenga un nivel de
red en el nodo.

Hibernación: En este estado no se reciben ni se transmiten datos; la etapa de
radiofrecuencia se apaga completamente para conservar la energía. Este estado finaliza
cuando termina el tiempo destinado al mismo (el cual es configurable y en adelante se le
llamará “T”). Una vez que finaliza se revisa si existen alarmas por enviar. Si hay alarmas
se pasa al primer estado de espera y en caso contrario a la primera fase de petición de
trama. Además, tiene un contador que lleva el control de
cuántas veces se ha pasado por este estado; Si este contador detecta que el protocolo ha
pasado más de X veces por este estado de manera consecutiva entonces el nodo regresa
al estado de descubrimiento del nivel de pertenencia para verificar la arquitectura de la
red y redefinir su nivel de pertenencia en la red. X será un parámetro de la red y se
definirá dependiendo de las necesidades de la misma, para los propósitos del presente
proyecto se definirá con un valor de 20.

Primera fase de petición de trama: En este estado el nodo escucha por un periodo de
tiempo 2B ( Donde “B” se define como el tiempo máximo que tarda la trama más larga del
protocolo en ser recibida a la distancia máxima de alcance de los nodos) para evitar
colisiones con tramas que se encuentren en proceso de transmisión por medio de la
escucha del canal en busca de tramas RTS (“Request to send” solicitud de envío), PT o
CTS (“Clear to send”, o libre para enviar). En caso de no escuchar tramas de CTS o RTS
envía una trama PT preguntado a los nodos de nivel superior si tienen una trama para
retransmitir; en caso de no recibir respuesta, el nodo vuelve al estado de hibernación.

Segunda fase de petición de trama: En caso de que alguna de las tramas PT descritas
en el estado anterior sea contestada con una trama RTS, el nodo pasa a la segunda fase
de petición de trama; en este estado el nodo envía una trama CTS para informar al primer
nodo que envió el RTS que tiene el canal asignado para la transmisión de la trama, los
nodos que escuchen el CTS se mantienen en silencio mientras se transmite el mensaje
con el objetivo de evitar que las tramas colisionen.

Escucha: Una vez que el nodo envía el CTS, espera para recibir el mensaje del nodo que
solicitó el canal; si el tiempo de espera se acaba, el nodo asume que el transmisor tuvo
un fallo y retorna al estado de hibernación. Si se recibe la trama a retransmitir, el nodo
pasa al estado de verificación.

Verificación: En este estado se envía una trama de ACK y se espera un tiempo para
determinar que la trama se haya recibido correctamente; si el nodo emisor de la trama
no corrige el mensaje enviado se asume que el mismo se recibió correctamente y se pasa
al primer estado de espera, en caso de que la trama sea corregida se guarda la nueva
información de la alarma y se inicia nuevamente el estado. La trama de ACK contiene un
código de CHECK SUM que permite verificar que la trama recibida en el receptor es la
correcta. Si la alarma que se recibe es correcta, se guarda y se pasa nuevamente al
estado de primera fase de petición de trama para ver si hay algún otro nodo que necesite
reenviar información.

Primer estado de espera: En este estado el nodo tiene alarmas por transmitir a la
base y espera por una trama PT de algún nodo de nivel inferior; en el momento en que
dicha trama es escuchada el nodo pasa al segundo estado de espera. Si el nodo escucha
una trama PT de un nodo de nivel inferior al inmediatamente inferior al propio, cambia
su identificador de nivel de red para ajustarse a la nueva configuración de la red. En caso
de no recibir una trama de PT válida después de un tiempo el nodo asume que hubo un
cambio en la red y vuelve al estado de descubrimiento de nivel sin borrar la memoria de
alarma.

Este estado tiene una doble función ya que además de tener un papel en el proceso de
transmisión de los datos también tiene el propósito de auto-configurar la red en caso de
que ocurra un cambio en la topología que la rodea. En caso de escuchar un CTS o un
RTS el nodo esperará por el tiempo requerido para que la transmisión en progreso sea
completada e ignorara cualquier PT que escuche en este tiempo.

Segundo estado de espera: Después de ingresar al segundo estado de espera el nodo
aguarda un tiempo aleatorio corto, esto con el objetivo de evitar colisiones con algún otro
nodo que desee contestar la transmisión PT, y envía una trama de RTS para luego pasar
al tercer estado de espera.

En caso de escuchar un CTS proveniente del nodo del que escuchó la trama de PT en
el periodo de espera, se incrementa un contador y el nodo regresa al primer estado de
espera. Si el contador indica que el nodo ha pasado por este estado 3 veces entonces en
lugar de pasar al primer estado de espera el nodo asume que hay muchos nodos que
desean transmitir sus tramas y pasa al estado de hibernación sin borrar la alarma.

Tercer estado de espera: En este estado el nodo espera por un CTS dirigido hacia él
mismo. En caso de ser recibido el nodo pasa al cuarto estado de espera. En caso de que
no reciba la trama CTS el nodo incrementa un contador que indica la cantidad de veces
que el nodo ha pasado por este estado y pasa al primer estado de espera. Si el contador
indica que el nodo ha pasado por este estado 3 veces entonces en lugar de pasar al
primer estado de espera el nodo asume que hay muchos nodos encendidos y pasa al de
hibernación sin borrar la alarma.

Cuarto estado de espera: En este estado el nodo envía la trama de alarma y espera
por una trama de ACK (de reconocimiento de envío) que indica que la información llegó
correctamente al nodo receptor; en caso de no recibirse después de un tiempo de espera
B, el nodo pasa al estado de hibernación sin limpiar la memoria de la alarma. En caso de
que el nodo reciba la trama de ACK se verifica que la información recibida sea correcta. Si
no es así se retransmite la trama y se espera nuevamente el ACK (o sea se vuelve a
iniciar el cuarto estado de espera). En caso de que la información sea recibida correctamente se
limpia la memoria de la alarma y se pasa al estado de hibernación.
Nodo base: La estación base solamente escucha para verificar las transmisiones de los
nodos de nivel uno y no detecta eventos. El diagrama de estados que se sigue en este
nodo se muestra en la figura 4


Figura 4: Ilustración de los estados seguidos por el nodo base

2.2 Algoritmo agrupamiento y mezcla de datos (AMD)
La naturaleza de la aplicación con la que se está trabajando hace que sea posible que
más de un nodo de la red detecte un evento al mismo tiempo, de manera que, por
ejemplo, ante el disparo de un arma de fuego, sean muchos los nodos que deseen
informar de la alarma en la misma área geográfica.

Con el objetivo de minimizar la cantidad de datos transitando en la red y por ende reducir
el consumo energético, se propone que los nodos, antes de usar el algoritmo de selección
de ruta de datos mezclados expuesto en el apartado anterior para dirigir la información
hacia los sumideros finales, compriman y mezclen la información que se enviará a los
sumideros en conglomerados locales. Para lograr lo anterior es necesario definir:

• Un algoritmo para formar los conglomerados locales y la cabeza del conglomerado.
• Un mecanismo para la compresión de los datos recolectados en las cabezas de conglomerados

Además, es necesario que el algoritmo propuesto sea compatible con el protocolo de
selección de ruta de datos mezclados con el objetivo de reducir o eliminar el uso de
tramas de control adicionales a las ya utilizadas en el protocolo de selección de ruta de
datos mezclados. Basados en lo anterior se propone que el algoritmo de agrupamiento y
mezcla de datos funcione igual que el algoritmo de selección de ruta de datos mezclados
con la diferencia que cada una de las cabezas de conglomerado funcionará como un
sumidero local en el que se acopiarán las alarmas, y se enviarán los mensajes usando el
algoritmo de selección de ruta de datos mezclados.


Figura 5: lustración de niveles del protocolo de selección de ruta de datos mezclados

2.2.1 Algoritmo para formar los conglomerados locales y seleccionar la cabeza del
conglomerado:

Asuma que se tiene una red en la que los nodos se encuentran posicionados como se
muestra en la figura 5. Lo primero que debe ocurrir en esta red es que cada nodo debe
descubrir su nivel en la red.

Una vez que cada nodo conoce su nivel de pertenencia en la red para el algoritmo de
selección de ruta de datos mezclados es necesario seleccionar las cabezas de
conglomerados, e iniciar el proceso de descubrimiento de nivel de pertenencia en el
algoritmo de agrupamiento y mezcla de datos. Para esto es necesario seleccionar un nivel
máximo de saltos permitidos en los conglomerados, al cual se le llamará: ”Max cluster
level” y de aquí en adelante se considerará como parámetro de la red.

Dependiendo de la cantidad de saltos máxima seleccionada se usarán los nodos de “nivel
de selección de ruta de datos mezclados” múltiplo de Max cluster level×2 como cabezas
de conglomerado.

En la figura 6 se muestra la manera en que se espera se distribuyan los niveles del
algoritmo de agrupamiento y mezcla de datos. En caso de que un nodo tenga varios
vecinos de nivel inferior en AMD usará para retransmitir al que tenga menor nivel en
ADM.


Figura 6: Ilustración de distribución de niveles del algoritmo de agrupamiento y mezcla de
datos para una red que usa max cluster level = 3

2.2.2 Algoritmo de compresión de datos
Para el presente proyecto se decide simplemente agrupar el tipo de alarma, seguido por
el número de nodos que reportaron la alarma y los identificadores; por ejemplo, se asume
que los nodos solamente tienen ocho bits de ID y que existen seis alarmas posibles a
transmitir en la red. Esto quiere decir que el campo de tipo de alarma solamente requerirá
tres bits, no obstante, se utilizarán ocho para dar la opción de agregar hasta 256
diferentes tipos de alarmas el del número de nodos que detectaron la alarma tendrá ocho bits y el de cada uno de los identificadores de nodo tendrá ocho bits respectivamente. En caso de que una cabeza de conglomerado reciba el
reporte de cuatro alarmas tipo uno de los nodos 01h, 03h, 05h y 07h, y además reciba dos
alarmas de tipo tres de los nodos 01h y 09h, los datos de la trama tendrían el siguiente
formato:

En el algoritmo propuesto cada nodo tiene la capacidad de comprimir los datos recibidos y
no solamente las cabezas de aglomerado. No obstante, al existir un mecanismo mediante
el cual los datos son todos enviados a un punto específico, esto da más oportunidad para
que los datos sean comprimidos-mezclados y se reducirá la cantidad de tramas
transitando la red, reduciendo de esta forma la probabilidad de colisión.

2.3 Algoritmo multicapa propuesto
Sumando lo expuesto en las secciones anteriores; a continuación: se detallan los estados
que tienen diferencias con respecto de lo expuesto en la sección donde se explican los
estados del ADM:

Descubrimiento de nivel de pertenencia: En esta etapa se hará lo mismo que se
explicó anteriormente solamente que el nodo descubre su nivel de pertenencia en la red
tanto para ADM como para AMD; para encontrar su nivel de AMD simplemente se
observa el nivel de ADM del nodo una vez que se obtiene, si el nivel es múltiplo de
max cluster level × 2 este se ubicará como cabeza de grupo, o sea en el nivel uno de
AMD, en caso contrario el nodo calculará la diferencia entre nivel de AMD del nodo y
el múltiplo de max cluster level ×2 más cercano y se ubicará en el nivel diferencia+1.
Por ejemplo: si max cluster level es igual a tres, y el nivel de ADM del nodo es igual
a siete, los múltiplos de max cluster level × 2 más cercanos son seis y doce, con una
diferencia de uno y cinco, por tanto, el nodo tomará un nivel de AMD igual a dos.

Primera fase de petición de trama: En este estado el nodo envía una trama preguntado
a los nodos de nivel superior si tienen una trama para retransmitir (de aquí en
adelante se le llamará a esta trama PT “petición de transmisión”). En caso de no recibir
respuesta, el nodo verifica si hay alarmas en la pila de alarmas y/o tramas en la pila de
tramas (la diferencia entre una trama y una alarma es que las tramas se re-transmiten
usando ADM mientras que las alarmas se retransmiten usando AMD). En caso de existir
tramas y alarmas, se procede a comprimir las alarmas y las tramas en un solo paquete
hasta alcanzar el tamaño máximo permitido por el protocolo. En caso de que la trama
resultante exceda dicho tamaño se crearán dos o más tramas para acomodar todos los
datos en una cantidad mínima de tramas. Una vez que los paquetes resultantes están
listos, se guardan en la pila de tramas y se pasa al primer estado de espera. En caso de
que solo existan alarmas, estas se comprimirán y se enviarán usando AMD en el primer
estado de espera. En caso de que no haya, ni tramas, ni alarmas en la pila, se retornará
al estado de hibernación.

2.4 Diseño de las tramas usadas en el protocolo

Para el diseño de las tramas se procura que las mismas tengan una cantidad mínima de
bits, esto con el propósito de minimizar el tiempo que duran las transmisiones y reducir
la probabilidad de colisión, así como el consumo de energía en el proceso de envío.
Cada uno de los espacios de información dentro de las tramas tendrá una longitud
predefinida, con excepción de las tramas de alarma, las cuales como se mencionó en la
sección 4.2.2, tendrán una longitud variable. Para efectos del presente ejemplo se asume
un byte para representar la mayoría de los campos en las tramas por simplicidad, no
obstante, estos tamaños pueden ser modificados para mejorar las características de la
red en una aplicación específica, por ejemplo, si se sabe que solamente se tendrán 8
tipos de tramas en la red entonces no necesario utilizar 8 bits para la cabecera de las
tramas sino solamente 3 bits. Las tramas requeridas para la implementación del
protocolo} anteriormente descrito son las siguientes:

Trama de PT: Esta trama contiene el nivel del nodo emisor con el objetivo de que los
nodos que la escuchen puedan verificar que la misma corresponde a un nodo más
cercano a la base que ellos mismos. Además, la trama debe contener la dirección MAC del emisor
para que el receptor sepa a quién debe hacer el RTS.

F1h Nivel_del_emisor_AMD Nivel_del_emisor_ADM Dirección_MAC_del_emisor

Trama de RTS: La trama de RTS debe contener la dirección MAC del emisor para que el
nodo que la escuche la incluya en la trama del CTS y este sepa que su solicitud fue
aceptada. Además, debe contener el nivel del nodo emisor para facilitar el proceso de
búsqueda de identidad de los nodos vecinos y el largo de la trama que se desea transmitir
para que, en caso de ser escuchado por algún nodo vecino, este sepa el periodo por el
cual debe esperar antes de intentar enviar su trama.

F2h Nivel_del_emisor_AMD Nivel_del_emisor_ADM Tamaño_trama_por_enviar Dirección_MAC_del_emisor

Trama de CTS : La trama CTS es una trama de difusión y por esto debe contener la
dirección MAC del nodo a la que va dirigida para que los demás nodos identifiquen a
quien se dirige y guarden silencio en caso de que no sea para ellos.

F3h Nivel_emisor_AMD Nivel_emisor_ADM Tamaño_trama_por_enviar Dirección_MAC_destinatario

Trama de alarma: Esta trama debe dejar claro el lugar de procedencia el tipo de alarma
registrado, por lo que debe contener el código de la alarma, así como la dirección MAC
del nodo en el que se produjo. Además, debe estar clasificada si debe ser enviada usando
ADM o AMD. En caso de que el nodo tenga el reporte de cuatro alarmas tipo uno de los
nodos 01h, 03h, 05h y 07h, y además tenga en cola dos alarmas de tipo tres de los nodos
01h y 09h, los datos de la trama tendrían el siguiente formato:

 0x05 nivel del ADC
 
Tabla 1: Descripción de los tipos de alarmas asignadas hasta el momento

El campo que se asigna al código de la alarma es de un byte dando de esta forma la
capacidad de comunicar 256 diferentes tipos de información, esto simplemente para dar
espacio para la adición de alarmas nuevas en caso de ser necesario. Se definen seis
tipos de alarmas por el momento, las cuales se muestran en la tabla 1.

La salida de operación del nodo se define como una alarma generada por el módulo de
alimentación e indica que el nodo se encuentra en un límite de carga que hace necesario
apagar el nodo para que se cargue hasta un nivel que permita su operación normal o si
no es posible recargar el nodo entonces para informar a la base que el nodo saldrá de
operación debido a carga insuficiente. Para indicar si la alarma debe transmitirse usando
ADM o AMD se utilizarán 2 cabeceras diferentes: F4 para ADM y F6 para AMD.

Trama de ACK: Esta trama debe dar la capacidad al nodo de saber si el dato fue
recibido correctamente.

0xF5 CHECK_SUM Dirección_MAC_de_la_procedencia

Se añade un campo para un CHECK SUM que para la implementación propuesta tendrá
8 bits y será simplemente los últimos 8 bits de la suma de los bits individuales del campo
de la información de la alarma, o sea, desde el byte de cabecera hasta antes del byte de
CHECK SUM. Se selecciona el anterior código de CHECK SUM por simplicidad, no
obstante queda al criterio del implementador del mismo el uso de algún otro método para
validar que el dato se recibió correctamente.

2.5 Diseño de los tiempos del protocolo

A continuación, se muestra un ejemplo de cómo definir los tiempos de espera e
hibernación asociados al protocolo propuesto, para una capa física en específico.
Para definir los tiempos del protocolo es necesario tener alguna referencia en cuanto al
consumo energético que un nodo de comunicación tendrá en escucha pasiva, transmisión
y en estado de hibernación, además de otros parámetros básicos que serán dependientes
de la capa física sobre la cual se implemente la red. Dado que no se han definido los
nodos físicos que se utilizarán para la implementación del presente proyecto en la red
de sensores de monitorización del bosque, y que para efectos del diseño del protocolo se
pretende obtener resultados independientes de la capa física, se tomará como referencia
el consumo energético de los módulos TR1000 usados también como referencia en [36].
Los datos de consumo de potencia obtenidos de estos nodos se utilizarán solamente para
obtener datos de referencia que posteriormente deberán ser repetidos usando datos que
correspondan con la capa física que se quiera usar para la implementación de la red.

Tiempo de hibernación: Este tiempo influye de manera directa en la cantidad de energía
consumida en la etapa de comunicación de los nodos. Entre más grande sea el tiempo
destinado en este estado, mayor será el ahorro en la energía del protocolo, no obstante,
el atraso en el proceso de envío de las tramas aumentará.
El consumo promedio de potencia de los módulos de comunicación se describe en la

tabla 2:

Tabla 2: Potencia promedio del TR1000 en los diferentes estados de la etapa de
comunicación. (Tomada de [36])

Tomando en cuenta que en un protocolo en el que mientras la red se encuentra en reposo
el módulo de comunicación se encuentra en estado de escucha se tiene que el consumo
de potencia en reposo es constante, y la potencia promedio será de 12,5 mW.
Figura 7: Gráfico aproximado del consumo de potencia esperado de los nodos con la red
en reposo utilizando el protocolo propuesto

En el caso de la red en reposo con el protocolo implementado el consumo de potencia no
será constante debido a que el módulo de comunicación pasará por diferentes estados a
lo largo de cada uno de los ciclos de petición de trama e hibernación. De esta manera el
consumo de energía en cada uno de los ciclos puede describirse aproximadamente como
se muestra en la figura 7 donde la potencia promedio será:

De lo anterior se puede ver que la potencia consumida será dependiente de la proporción
entre los tiempos P y T, si la red se encuentra en reposo (sin recibir señales de alarma) y
se gasta un tiempo máximo aproximado de P (2B) en la primera fase de petición de trama,
el porcentaje de ahorro de energía en el nodo con respecto de un protocolo en el que la
etapa de radio (transmisión, recepción) debe mantenerse permanentemente encendida,
se comportará aproximadamente como se muestra en la Figura 8 conforme aumenta el
tiempo de hibernación


Figura 8: En el eje vertical se ve el porcentaje de ahorro en la energía para la red en
reposo

en la etapa de comunicación con respecto de un protocolo en el que esta etapa se
mantiene constantemente encendida, mientras que en el eje horizontal se ve que
tan grande es el tiempo T con respecto a P

El hecho de que el ahorro energético se ve acotado en aproximadamente 99,87% se debe
a que para este caso la diferencia entre la potencia consumida en la etapa de sueño
profundo y la potencia consumida en escucha pasiva es de 12,48mW lo cual es 99,87%
de la potencia consumida en estado de escucha, de forma que conforme aumente el
tiempo de hibernación la potencia consumida en el nodo tenderá a la potencia consumida
con la etapa de trasmisión apagada por lo que el ahorro máximo del protocolo con
respecto de un protocolo en estado que no apaga su etapa de radio será menor que de 99,87% para
el presente ejemplo.

Para una red con N niveles en los que se programa un tiempo de hibernación T y la suma
de todos los demás tiempos de espera es K, sin retransmisiones se tendrá que pasar
como máximo por un proceso de agrupamiento de datos hacia una cabeza de conglomerado.

En el peor de los casos se debe sumar al número de saltos Max cluster level − 1, tomando
en cuenta que cada uno de los nodos opera sin sincronización. El tiempo máximo (en el
peor de los casos) entre la detección de una alarma y su recepción en la estación base
será:

Donde Tmax es el tiempo de latencia máximo y Tbase es el tiempo de ciclo base. Dado
que el tiempo más grande contemplado en la red es el de hibernación, se tiene que el tiempo
entre la emisión de una alarma y su recepción en la estación base será claramente
afectado por el mismo.

Descubrimiento de nivel de pertenencia: El tiempo que se escucha en este estado
debe ser lo suficientemente largo como para que el nodo escuche las tramas PT de todos
sus vecinos y al mismo tiempo no debe ser más largo de lo necesario para que no se
desperdicie energía.

Si se supone que la red se encuentra en reposo y ya ha sido configurada, entonces cada
uno de los nodos transmitirá una trama PT aproximadamente cada vez que se termina
el tiempo de hibernación; para asegurar que los nodos tengan el tiempo suficiente para
escuchar las tramas PT de todos sus vecinos se asigna en este estado un tiempo de
espera máximo del doble del tiempo de hibernación, o sea 2T, en caso de que después de 2T el
nodo no escuche ninguna trama de PT entonces el nodo pasaré al estado de hibernación
y volverá al estado descubrimiento de nivel de pertenencia al terminar la hibernación.
Primer estado de espera: En este estado, al igual que en el estado de descubrimiento
de nivel es necesario esperar tiempo máximo suficiente para escuchar las PT de sus
vecinos o sea 2T si después de este tiempo no se escucha ninguna trama de PT válida,
entonces se pasará al estado de descubrimiento del nivel de pertenencia.

Segundo estado de espera: En este estado se espera un tiempo aleatorio con el
propósito de evitar colisiones después de haber escuchado una trama de PT de un nodo
de nivel inferior. El tiempo aleatorio que se espera antes de enviar la trama debe ser lo
más corto posible para que no se gaste mucha energía, no obstante, entre más sean los
posibles tiempos de transmisión en este estado, menor será la probabilidad de colisión.
Este estado influye directamente en el tiempo que debe ser asignado en la espera entre
cada una de las tramas de PT de la primera fase de petición de trama y con esto influye
directamente en el tiempo que debe ser asignado al estado de hibernación y al tiempo de
retraso total de las tramas transmitidas. Tomando en cuenta todo lo anterior este tiempo
se define entre cero y 8B, de esta forma

el nodo tiene cinco tiempos posibles en los cuales puede intentar transmitir su trama de
RTS; en t = 0, t = 2B, t = 4B , t = 6B y t = 8B esto con el objetivo de que si otro nodo
transmite en un tiempo menor el nodo que se encuentra esperando tenga tiempo
suficiente para escuchar la trama de CTS y evite la colisión.

Primera fase de petición de trama: En este estado es necesario implementar un
tiempo de espera antes de trasmitir la trama de PT para evitar que la trama corrompa
alguna transmisión en proceso por no haber escuchado otras tramas de PT, CTS o RTS.
Este tiempo debe ser aproximadamente 2B. Una vez que se transmite la trama de PT la
estación debe esperar por 9B en el peor de los casos para dar tiempo a que la estación
que desea reenviar su alarma transmita su RTS. Se puede notar que el tiempo P
anteriormente usado para modelar el ahorro energético utilizando estados de hibernación
será de aproximadamente 11B más el tiempo requerido para transmitir la trama de PT.
En el caso de los nodos de referencia, el tiempo requerido para la transmisión de la trama
será de aproximadamente 4,5ms, lo cual es despreciable si se compara los 58ms de la
base de tiempo B por lo que de aquí en adelante se dirá que el tiempo P es de 11B.
Estado de escucha: En este estado se espera un tiempo máximo de 2B después de
haber enviado la trama de CTS para recibir el mensaje del nodo al que se le notificó, o
para volver al estado de hibernación.

Verificación: En este estado se espera un tiempo máximo de 2B antes de dar por
aceptado como correcto el mensaje recibido después de enviar la trama de ACK
Tercer estado de espera: Al igual que en el caso anterior el nodo esperará 2B para
recibir un CTS. Cuarto estado de espera: En este estado el nodo espera un tiempo 2B
para recibir la trama de ACK.
