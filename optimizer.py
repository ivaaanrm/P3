import subprocess
import numpy as np
import shlex

# umaxnorm = 0.72
# minZcr = 3000
# u1norm = 0.953
# maxpot = -38

umaxnorm_ = np.arange(0.55, 0.75, 0.01)
minZcr_ = np.arange(2000, 4000, 200)
u1nomr_ = np.arange(0.4, 1, 0.2)
maxpot_ = np.arange(-60, -20, 10)
alpha_ = np.arange(0.00040,0.00060,0.00005)


# proc = subprocess.run(["make", "release"])
# for alpha in alpha_:
#     for maxpot in maxpot_:
#         for u1norm in u1nomr_:
#             for minZcr in minZcr_:
#                 for umaxnorm in umaxnorm_:
#                     proc = subprocess.run(["run_get_pitch", "-m", f"{umaxnorm}", "-z", f"{minZcr}", "-u",f"{u1norm}", "-p",f"{maxpot}", "-a", f"{alpha}" ])
#                     # proc = subprocess.run(shlex.split("pitch_evaluate pitch_db/train/*f0ref"))
#                     proc1 = subprocess.Popen("pitch_evaluate pitch_db/train/*f0ref", shell=True, stdout=subprocess.PIPE).stdout.read()

#                     res_str = str(proc1).split("TOTAL")[-1]
#                     res = float(res_str.split(":\\t")[1].split(" ")[0])

#                     print(f"Params: umaxnorm: {umaxnorm} | zcr: {minZcr} | u1nomr: {u1norm} | maxpot: {maxpot} | alpha: {alpha}")
#                     print(f"TOTAL --> {res}")
#                     print(15*"-")
 


for alpha in alpha_:
    for maxpot in maxpot_:
        for u1norm in u1nomr_:
            for minZcr in minZcr_:
                for umaxnorm in umaxnorm_:
                    print(5*"*" + f"{umaxnorm} | {minZcr} | {u1norm} | {maxpot} | {alpha}")
                    proc = subprocess.run(["get_pitch", "prueba.wav","prueba.f0", "-m", 
                        f"{umaxnorm}", "-z", f"{minZcr}", "-u", f"{u1norm}", "-p", f"{maxpot}","-a", f"{alpha}"])
                    proc2 = subprocess.run(["pitch_evaluate", "prueba.f0ref"])

print("The exit code was: %d" % proc.returncode)


