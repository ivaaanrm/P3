PAV - P3: estimación de pitch
=============================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la estimación de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.
   
```cpp
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {
  		/// \TODO Compute the autocorrelation r[l]
      r[l]= 0.0f;
      for(unsigned int n = l; n<x.size();n++){
        r[l] += x[n]*x[n-l];
      }
      r[l] = r[l] / x.size();
    }
    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
  }
```

   * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un segmento de
     unos 30 ms de un fonema sonoro y su periodo de pitch; y, en otro *subplot*, se vea con claridad la
	 autocorrelación de la señal y la posición del primer máximo secundario.
	 
	 ![sonoro](https://user-images.githubusercontent.com/82904867/199552097-0bd7f51c-c4dc-4c50-a270-e1f0c30c2b44.png)


	 NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
	 hacerlo. Se valorará la utilización de la biblioteca matplotlib de Python.

   * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la
     autocorrelación. Inserte a continuación el código correspondiente.

```cpp
for (iR = iRMax = r.begin() + npitch_min; iR < r.begin() + npitch_max; iR++) {
      if (*iRMax < *iR) iRMax = iR;
    }
```

   * Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.

```cpp
 bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm, float zcr) const {
    if((pot < maxpot && r1norm < u1norm) || (rmaxnorm < umaxnorm || zcr > minZcr)){
      return true;
    } else {
      return false;
    }
  }
```

   * Puede serle útil seguir las instrucciones contenidas en el documento adjunto `código.pdf`.

- Una vez completados los puntos anteriores, dispondrá de una primera versión del estimador de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la estimación de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		(r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).

<img width="1439" alt="Screenshot 2022-11-03 at 12 53 25" src="https://user-images.githubusercontent.com/67743899/199715709-6242166b-b730-4cb4-802d-8e61714caa91.png">

![Figure_1](https://user-images.githubusercontent.com/82904867/202670072-6c7e8221-ce75-45fe-aa81-62fc8e3ad7f8.png)

**La tasa de cruces por cero y la autocorrelación en el segundo máximo son los dos parametros más relevantas para detectar si el señal es sordo o sonoro.
Por tanto el objetivo será optimizar estos dos parámetros**

![Figure_3](https://user-images.githubusercontent.com/82904867/202676426-08962d2c-19f2-4792-81e7-12a6deb3cf46.png)



Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

```cpp
  int sgn(float x) {
    if (x > 0.0) return 1;
    if (x < 0.0) return -1;
    return 0;
  }

  float PitchAnalyzer::compute_zcr(std::vector<float> & x) const {
    int N = x.size();
    float fm = samplingFreq;
    float sum = 0.0;
    for (int i = 0; i < N; i++){
        if (sgn(x[i]) != sgn(x[i-1])){
            sum++;
        }
    }
    return fm/(2*(N-1))*sum;
  }
```

Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
en esta práctica es de 15 ms.

- Use el estimador de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica ilustrativa del resultado de ambos estimadores. 
    Aunque puede usar el propio Wavesurfer para obtener la representación, se valorará el uso de alternativas de mayor calidad (particularmente Python).
		
![Figure_2](https://user-images.githubusercontent.com/82904867/202551559-ac9fd0d3-6394-49f7-b23a-db26b2a0509b.png)

  * Optimice los parámetros de su sistema de estimación de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..

![image](https://user-images.githubusercontent.com/82904867/202923581-3b02ff81-e747-412c-9f0e-239255be1088.png)


Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  estimador a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del estimador. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la estimación de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.

![image](https://user-images.githubusercontent.com/82904867/202923624-5c013ea8-0954-45ff-b041-cb9049860e59.png)


![image](https://user-images.githubusercontent.com/82904867/202845046-fe24b0b4-4a66-4f19-91c4-fb922491879c.png)


- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de estimación
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:
  * Enventanado de Hamming
  
  **Depués de hacer pruebas con el enventanado de Hamming el score final de predicción era inferior al de la ventana rectanglar. Por tanto finalmente hemos usado el enventanado original.**

```cpp
case HAMMING:
	for (unsigned int n = 0; n < frameLen; n++){
	window[n] = 0.54 - 0.46 * cos(2 * M_PI * n / (frameLen - 1));
	}
break;
```
  
  * Técnicas de preprocesado: filtrado paso bajo, diezmado, *center clipping*, etc.
  
  **Como técnica de preprocesado hemos decidio usar center clipping que consiste en anular los valores de x[n] de magnitud pequeña.**
   ```cpp
  vector<float>::iterator iX;
  float f = 0.0;
  // float alpha = 0.00049;
  for (iX = x.begin(); iX < x.end(); iX++) {
    if (*iX < alpha && *iX > -alpha){ 
      *iX = 0.0;
    }
  }
```
  
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  
   **Como técnica de postprocesado hemos decidido crear un filtro que corrige valores nulos en medio tramos sonoros y valores de pitch en medio de tramos sordos. Con este método obtenemos mejores resultados que con el filtro de mediana.**
  
```cpp
  float avgPitch = 0;
  int numPitch = 0;

  // filter that eliminates the single voiced frames
  vector<float>::iterator iF0;
  for (iF0 = f0.begin(); iF0 < f0.end(); iF0++) {
    if (*iF0 != 0.0) {
      avgPitch += *iF0;
      numPitch++;
      if (iF0 == f0.begin()) {
        if (*(iF0 + 1) == 0.0) {
          *iF0 = 0.0;
        }
      } else if (iF0 == f0.end() - 1) {
        if (*(iF0 - 1) == 0.0) {
          *iF0 = 0.0;
        }
      } else {
        if (*(iF0 - 1) == 0.0 && *(iF0 + 1) == 0.0) {
          *iF0 = 0.0;
        }
      }
    }
  }

  // filter that fills the single unvoiced frames
  for (iF0 = f0.begin(); iF0 < f0.end(); iF0++) {
    if (*iF0 == 0.0) {
      if (iF0 == f0.begin()) {
        if (*(iF0 + 1) != 0.0) {
          *iF0 = *(iF0 + 1);
        }
      } else if (iF0 == f0.end() - 1) {
        if (*(iF0 - 1) != 0.0) {
          *iF0 = *(iF0 - 1);
        }
      } else {
        if (*(iF0 - 1) != 0.0 && *(iF0 + 1) != 0.0) {
          *iF0 = (*(iF0 - 1) + *(iF0 + 1)) / 2;
        }
      }
    }
  }
  
      // filter that fills double zero holes
  for (iF0 = f0.begin(); iF0 < f0.end()-3; iF0++) {
    if (*iF0 != 0.0 && *(iF0 + 1) == 0.0 && *(iF0 + 2) == 0.0 && *(iF0 + 3) != 0.0) {
      *(iF0 + 1) = (*(iF0) + *(iF0 + 3)) / 2;
      *(iF0 + 2) = (*(iF0) + *(iF0 + 3)) / 2;
    }

  avgPitch = avgPitch / numPitch;

// valores de pitch por encima de un umbral los corregimos al velor de la media
  for (iF0 = f0.begin(); iF0 < f0.end(); iF0++) {
    if (*iF0 >= avgPitch*1.46){
      *iF0 = avgPitch*0.81;
    }
  }
  

  }
```
  
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el estimador, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
    
    **Para la optimización de parametros hemos modificado el run_get_pitch para poder pasar parametros. Con python ejecutamos el programa iteradamente optimizando los parametros usando toda la base de datos de entrenamiento**
    
    ```python
    """ optimizer.py """
    with open("opt-res.log", "a") as f:
    for umaxnorm in umaxnorm_:
        for minZcr in minZcr_:
            for u1norm in u1norm_:
                for maxpot in maxpot_:
                    for alpha in alpha_:

                        proc = subprocess.run(["run_get_pitch", "-m", f"{umaxnorm}", "-z", f"{minZcr}", "-u",f"{u1norm}", 
							"-p",f"{maxpot}", "-a", f"{alpha}" ])
                        proc1 = subprocess.Popen("pitch_evaluate pitch_db/train/*f0ref", shell=True, stdout=subprocess.PIPE).stdout.read()

                        res_str = str(proc1).split("TOTAL")[-1]
                        res = float(res_str.split(":\\t")[1].split(" ")[0])

                        if res >= max_res["Total"]:
                            max_res["Total"] = res
                            max_res["umaxnorm"] = umaxnorm
                            max_res["zcr"] = minZcr
                            max_res["u1"] = u1norm
                            max_res["pot"] = maxpot
                            max_res["alpha"] = alpha
                        
                        progress.append(max_res["Total"])

                        params = f"Params: umaxnorm: {umaxnorm} | zcr: {minZcr} | u1nomr: {u1norm} | maxpot: {maxpot} | alpha: {alpha}\n"
                        result = f"({i}/{totales})TOTAL --> {res}\n------------------\n"
                        f.write(params)
                        f.write(result)
                        print(params)
                        print(result)
                        i = i + 1
    
    ```
    
    Posteriormente filtramos para obtener el mejor resultado
    
    ````
	    Params: umaxnorm: 0.378 | zcr: 3430 | u1nomr: 0.975 | maxpot: -46.5 | alpha: 0.003
	(591/3600)TOTAL --> 91.72
	------------------
	Params: umaxnorm: 0.378 | zcr: 3430 | u1nomr: 0.975 | maxpot: -46.5 | alpha: 0.0035
	(592/3600)TOTAL --> 91.64
	------------------
	Params: umaxnorm: 0.378 | zcr: 3440 | u1nomr: 0.975 | maxpot: -46.5 | alpha: 0.002
	(593/3600)TOTAL --> 91.54
	------------------
	Params: umaxnorm: 0.378 | zcr: 3440 | u1nomr: 0.975 | maxpot: -46.5 | alpha: 0.0025
	(594/3600)TOTAL --> 91.61
	------------------
	Params: umaxnorm: 0.378 | zcr: 3440 | u1nomr: 0.975 | maxpot: -46.5 | alpha: 0.003
	(595/3600)TOTAL --> 91.73
	------------------
	Params: umaxnorm: 0.378 | zcr: 3440 | u1nomr: 0.975 | maxpot: -46.5 | alpha: 0.0035
	(596/3600)TOTAL --> 91.65
	------------------
	Params: umaxnorm: 0.378 | zcr: 3450 | u1nomr: 0.975 | maxpot: -46.5 | alpha: 0.002
	(597/3600)TOTAL --> 91.54
	------------------
	Params: umaxnorm: 0.378 | zcr: 3450 | u1nomr: 0.975 | maxpot: -46.5 | alpha: 0.0025
	(598/3600)TOTAL --> 91.62
	------------------
    ````
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al estimador. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.
  
  En la siguiente gráfica se muestra la prograsión de mejora por cada iteración al optimizar los parámetros.
  
  ![Figure_1](https://user-images.githubusercontent.com/82904867/202923907-9c29e2c0-be85-4763-a5c8-e66e307cb997.png)


  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   
   
Además, hemos creado un jupyter notebook para poder evaluar rápidamente nuestros resultados desde cualquier dispositivo y lugar, puesto que está en un google colab:

   https://colab.research.google.com/drive/1WJZ3Tlqc9RwIs1UxhHoXZbPcTQq3Kwwj?usp=sharing#scrollTo=BB6moQ3v61Z0

Evaluación *ciega* del estimador
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
estimador con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
