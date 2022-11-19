
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
zcr = [float(dat[3]) for dat in datContent]


n = np.arange(0, len(pitch_estimate))

#Plots
fig1, axs = plt.subplots(5,1)
fig1.set_size_inches(12,10)



axs[0].set_title("potencia de la señal")
axs[0].set_ylabel("")
axs[0].set_xlabel("")
axs[0].grid(True)
axs[0].plot(n,power)
axs[0].hlines(y=-38, xmin=0, xmax=200, linewidth=1, color='r')


axs[1].set_title("autocorrelación normalizada de uno")
axs[1].set_ylabel("")
axs[1].set_xlabel("")
axs[1].grid(True)
axs[1].plot(n,r1norm)
axs[1].hlines(y=0.953, xmin=0, xmax=200, linewidth=1, color='r')


axs[2].set_title("autocorrelación en su máximo secundario")
axs[2].set_ylabel("")
axs[2].set_xlabel("")
axs[2].grid(True)
axs[2].plot(n,rmaxnorm)
axs[2].hlines(y=0.38, xmin=0, xmax=200, linewidth=1, color='r')
# axs[2].vlines(x=40, ymin=0, ymax=1, linewidth=1, color='r')
# axs[2].vlines(x=66, ymin=0, ymax=300, linewidth=1, color='r')
# axs[2].vlines(x=96, ymin=0, ymax=300, linewidth=1, color='r')

axs[3].set_title("tasa de cruces por cero")
axs[3].set_ylabel("")
axs[3].set_xlabel("")
axs[3].grid(True)
axs[3].plot(zcr)
axs[3].hlines(y=2300, xmin=0, xmax=200, linewidth=1, color='r')


axs[4].set_title("Pitch referencia")
axs[4].set_ylabel("")
axs[4].set_xlabel("")
axs[4].grid(True)
axs[4].plot(n,pitch_ref)


# for i in range(5):
#     axs[i].vlines(x=40, ymin=0, ymax=1, linewidth=1, color='r')
#     axs[i].vlines(x=66, ymin=0, ymax=300, linewidth=1, color='r')
#     axs[i].vlines(x=96, ymin=0, ymax=300, linewidth=1, color='r')



plt.tight_layout()


# fig2, axs = plt.subplots(2,1)
# # fig2.set_size_inches(10,10)

# axs[0].set_title("Pitch estimado")
# axs[0].set_ylabel("")
# axs[0].set_xlabel("")
# axs[0].grid(True)
# axs[0].plot(pitch_estimate)

# axs[1].set_title("Pitch referencia")
# axs[1].set_ylabel("")
# axs[1].set_xlabel("")
# axs[1].grid(True)
# axs[1].plot(pitch_ref)

# plt.tight_layout()

""" Save zcr data in file"""
# with open("rmaxnorm.dat", "w") as f:
#     for i in rmaxnorm:
#         f.write(str(i)+"\n")



# fig3, axs = plt.subplots(3,1)
# fig3.set_size_inches(8,6)

# axs[0].set_title("Pitch referencia")
# axs[0].set_ylabel("")
# axs[0].set_xlabel("")
# axs[0].grid(True)
# axs[0].scatter(n,pitch_ref)

# axs[1].set_title("autocorrelación en su máximo secundario")
# axs[1].set_ylabel("")
# axs[1].set_xlabel("")
# axs[1].grid(True)
# axs[1].plot(n,rmaxnorm,n,pitch_estimate)

# axs[2].set_title("tasa de cruces por cero")
# axs[2].set_ylabel("")
# axs[2].set_xlabel("")
# axs[2].grid(True)
# axs[2].plot(zcr)
# plt.tight_layout()


plt.show()
