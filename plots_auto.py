
import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf


signal, fm = sf.read("prueba.wav")

#Temps 
t = np.arange(0, len(signal))

print(fm)

datContent = [i.strip().split() for i in open("out.dat").readlines()]

pitch_estimate = [float(i) for i in open("prueba.f0"). readlines()]
pitch_ref = [float(i) for i in open("prueba.f0ref"). readlines()]

power = [float(dat[0]) for dat in datContent]
r1norm = [float(dat[1]) for dat in datContent]
rmaxnorm = [float(dat[2]) for dat in datContent]

print(len(pitch_ref))

#Plots
fig1, axs = plt.subplots(3,1)
fig1.set_size_inches(8,6)

axs[0].set_title("potencia de la señal")
axs[0].set_ylabel("")
axs[0].set_xlabel("")
axs[0].grid(True)
axs[0].plot(power)

axs[1].set_title("autocorrelación normalizada de uno")
axs[1].set_ylabel("")
axs[1].set_xlabel("")
axs[1].grid(True)
axs[1].plot(r1norm)

axs[2].set_title("autocorrelación en su máximo secundario")
axs[2].set_ylabel("")
axs[2].set_xlabel("")
axs[2].grid(True)
axs[2].plot(rmaxnorm)
plt.tight_layout()


fig2, axs = plt.subplots(2,1)
# fig2.set_size_inches(10,10)

axs[0].set_title("Pitch estimado")
axs[0].set_ylabel("")
axs[0].set_xlabel("")
axs[0].grid(True)
axs[0].plot(pitch_estimate)

axs[1].set_title("Pitch referencia")
axs[1].set_ylabel("")
axs[1].set_xlabel("")
axs[1].grid(True)
axs[1].plot(pitch_ref)

plt.tight_layout()
plt.show()

""" Save zcr data in file"""
with open("rmaxnorm.dat", "w") as f:
    for i in rmaxnorm:
        f.write(str(i)+"\n")