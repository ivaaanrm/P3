import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf


#Llegim l'audio
signal, fm = sf.read("sonoro.wav")

#Temps 
t = np.arange(0, len(signal)) / fm  
                                                 
#Calculem autocorrelació i axis
r = np.correlate(signal, signal, "full")
r = r / r[int(len(r)/2)] 
raxis = np.arange(len(r))




# """ Save zcr data in file"""
# with open("zcr.dat", "w") as f:
#     for i in power:
#         f.write(str(i)+"\n")


#Plots
fig, axs = plt.subplots(2,1)
fig.set_size_inches(10,10)
axs[0].set_title("Señal original")
axs[0].set_ylabel("")
axs[0].set_xlabel("time")
axs[0].grid(True)
axs[0].plot(t, signal)

axs[1].set_title("Autocorrelation")
axs[1].set_ylabel("")
axs[1].set_xlabel("samples")
axs[1].grid(True)
axs[1].plot(raxis,r)

plt.show()



