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
    float minZcr = 5000.0;
    if((pot < maxpot || r1norm < u1norm) && rmaxnorm < umaxnorm || zcr > minZcr){
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

![image](https://user-images.githubusercontent.com/82904867/202567190-69d3aa62-4180-4f0e-a246-3b202ac64fef.png)


Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  estimador a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del estimador. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la estimación de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.
    
    ![image](https://user-images.githubusercontent.com/82904867/202566452-fa8def99-478e-412a-8718-0e9a0675a5d8.png)
![image](https://user-images.githubusercontent.com/82904867/202570027-64760fd3-8d25-4bc8-a129-886995fc54f1.png)


- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de estimación
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:
  * Enventanado de Hamming

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
  vector<float> f0;
  float alpha = 0.00071;
  float f = 0.0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift){
    // anulamos los valores de x[n] de magnitud pequeña.
    if (*iX<alpha && * iX> - alpha){
      f = 0.0;
    }
    else{
      f = analyzer(iX, iX + n_len);
    }
    f0.push_back(f);
  }

```
  
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  
   **Como técnica de preprocesado hemos decidio usar filtros de mediana con el objetivo de eliminar valores nulos en medio tramos sonoros y valores de pitch en medio de tramos sordos.**
  
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

  avgPitch = avgPitch / numPitch;

  for (iF0 = f0.begin(); iF0 < f0.end(); iF0++) {
    if (*iF0 >= avgPitch*1.46){
      *iF0 = avgPitch*0.81;
    }
  }
```
  
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el estimador, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al estimador. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.

  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   

Evaluación *ciega* del estimador
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
estimador con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
