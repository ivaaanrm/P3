import subprocess
import numpy as np
import matplotlib.pyplot as plt
import shlex

umaxnorm_ = [0.405]
minZcr_ = [3480]
u1norm_ = [0.975]
maxpot_ = [-46.5]
# alpha_ = [0.00049]

umaxnorm_ = np.arange(0.37, 0.42, 0.002)
minZcr_ = np.arange(3400, 3760, 10)
# u1nomr_ = np.arange(0.97, 1, 0.01)
# maxpot_ = np.arange(-47, -45, 0.5)
alpha_ = np.arange(0.002,0.004,0.0005)

iteraciones = 0;

max_res = {
    "Total": 0,
    "umaxnorm": 0,
    "zcr": 0,
    "u1": 0,
    "pot": 0
}

progress = []
# proc = subprocess.run(["make", "release"])
i = 1
totales = len(umaxnorm_)*len(u1norm_)*len(minZcr_)*len(maxpot_)*len(alpha_)

""" Optimmizacion de los parametros sobre la base de datos de entrenamiento """
with open("opt-res.log", "a") as f:
    for umaxnorm in umaxnorm_:
        for minZcr in minZcr_:
            for u1norm in u1norm_:
                for maxpot in maxpot_:
                    for alpha in alpha_:

                        proc = subprocess.run(["run_get_pitch", "-m", f"{umaxnorm}", "-z", f"{minZcr}", "-u",f"{u1norm}", "-p",f"{maxpot}", "-a", f"{alpha}" ])
                        # proc = subprocess.run(shlex.split("pitch_evaluate pitch_db/train/*f0ref"))
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
 

""" Optimmizacion de los parametros sobre un señal en concreto (prueba.wav) """
# for alpha in alpha_:
#     for maxpot in maxpot_:
#         for u1norm in u1nomr_:
#             for minZcr in minZcr_:
#                 for umaxnorm in umaxnorm_:
#                     print(5*"*" + f"{umaxnorm} | {minZcr} | {u1norm} | {maxpot} | {alpha}")
#                     proc = subprocess.run(["get_pitch", "prueba.wav","prueba.f0", "-m", 
#                         f"{umaxnorm}", "-z", f"{minZcr}", "-u", f"{u1norm}", "-p", f"{maxpot}","-a", f"{alpha}"])
#                     proc2 = subprocess.run(["pitch_evaluate", "prueba.f0ref"])

 
# for alpha in alpha_:
#     for maxpot in maxpot_:
#         for u1norm in u1nomr_:
#             for minZcr in minZcr_:
#                 for umaxnorm in umaxnorm_:
#                     print(5*"*" + f"{umaxnorm} | {minZcr} | {u1norm} | {maxpot} | {alpha}")
#                     proc = subprocess.run(["/root/PAV/bin/get_pitch", "prueba.wav","prueba.f0", "-m", 
#                         f"{umaxnorm}", "-z", f"{minZcr}", "-u", f"{u1norm}", "-p", f"{maxpot}","-a", f"{alpha}"])
#                     proc2 = subprocess.run(["/root/PAV/bin/pitch_evaluate", "prueba.f0ref"])

print("The exit code was: %d" % proc.returncode)

print(max_res)


# plot the data
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
ax.plot(progress, color='tab:blue')
plt.show()

