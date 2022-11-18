#!/bin/bash


while [[ $# > 0 ]]  # Itero sobre la cantidad de parametros que se ingresaron.
do
    case "$1" in
        -m | --param1 )
            shift  # Una vez que se encuentra el patron "-p1" o "--param1"
                   # Se recorre el indice del array de argumentos.
            declare param1="$1"  # Entonces el siguiente valor de entrada se almacena en la variable "param1"
            shift  # Y se vuelve a recorrer para proceder con este método.
        ;;
        -z | --param2 )
            shift  # Una vez que se encuentra el patron "-p1" o "--param1"
                   # Se recorre el indice del array de argumentos.
            declare param2="$1"  # Entonces el siguiente valor de entrada se almacena en la variable "param1"
            shift  # Y se vuelve a recorrer para proceder con este método.
        ;;
        -u | --param3 )
            shift  # Una vez que se encuentra el patron "-p1" o "--param1"
                   # Se recorre el indice del array de argumentos.
            declare param3="$1"  # Entonces el siguiente valor de entrada se almacena en la variable "param1"
            shift  # Y se vuelve a recorrer para proceder con este método.
        ;;
        -p | --param4 )
            shift
            declare param4="$1"
            shift
        ;;
        -a | --param5 )
            shift
            declare param5="$1"
            shift
        ;;
        * ) 
            # En caso de no coincidir, de igual forma se recorre el indice para
            # continuar con el bucle.
            shift
        ;;
    esac        
done 
# Put here the program (maybe with path)
GETF0="/root/PAV/bin/get_pitch -m $param1 -z $param2 -u $param3 -p $param4 -a $param5"

for fwav in pitch_db/train/*.wav; do    
    ff0=${fwav/.wav/.f0}
    # echo $ff0
    # echo "$GETF0 $fwav $ff0 ----"
    $GETF0 $fwav $ff0 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
done

exit 0
