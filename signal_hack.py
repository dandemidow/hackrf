import numpy as np
import matplotlib.pyplot as plt
from numpy import tri
from numpy._core.strings import lower

print("hello matlab")

f_mod = 440
F = 3000
sr = 10000000
state = 0
samples_generated = 0

def fir_filter(idx, input):
    N = 63
    filter_index = np.array([idx for idx in range(N)])
    low_pass_coef = np.sin(2 * np.pi * (F + 1000) / sr * filter_index) * 100 / (filter_index+1) / np.pi
    # print(low_pass_coef)
    sig = np.zeros(N)
    for i in range(N):
        if i < idx:
            sig[i] = input[idx - i]
    a = low_pass_coef * sig
    return np.sum(a)


def triang(one: int):
    global state
    global samples_generated
    period_in_samples = sr / f_mod
    step = 4.0 / period_in_samples

    if samples_generated < period_in_samples / 2:
        # state += step
        state = 0.75
    else:
        # state -= step
        state = 1.25
    samples_generated = samples_generated + 1
    if samples_generated >= period_in_samples:
        samples_generated = 0

    # return state - 1.0
    return state

dt = 1 / sr
t = np.arange(0, 0.005, dt)
o = np.ones(len(t))
k = np.array([idx for idx in range(len(t))])
arg = np.array([triang(idx) for idx in range(len(t))])
se = np.exp(1j * 2 * np.pi * F / sr * arg * k)
s1 = se.real
s2 = se.imag

base_inphase = 2 * np.cos(2 * np.pi * F / sr * k)
base_quadrature = -2 * np.sin(2 * np.pi * F / sr * k)

mult_inphase = s1 * base_inphase / 10
mult_quadrature = s1 * base_quadrature / 10

fir_inphase = np.array([fir_filter(idx, mult_inphase) for idx in range(len(t))])
fir_quadrature = np.array([fir_filter(idx, mult_quadrature) for idx in range(len(t))])

print("{}".format(arg))
print("{}".format(s1))

mod_inphase = np.cos(2 * np.pi * F / sr * k)
mod_quadrature = 0 - np.sin(2 * np.pi * F / sr * k)

sig_inphase = np.ones(len(t))
sig_quadrature = np.zeros(len(t))
ui = np.cos(2 * np.pi * 3000 / sr * k)
uq = np.sin(2 * np.pi * 3000 / sr * k)
word = int(sr / F)
for i in range(3 * word, 10 * word):
    sig_inphase[i] = ui[i]
    sig_quadrature[i] = uq[i]

# s = mod_inphase * fir_inphase + mod_quadrature * fir_quadrature
s = sig_inphase * mod_inphase + sig_quadrature * mod_quadrature

fig, axs = plt.subplots(2, 1)

axs[0].plot(t, s1, color='tab:blue')
axs[0].plot(t, s / 2, color='tab:red')

axs[1].plot(t, fir_inphase, color='tab:red')
axs[1].plot(t, fir_quadrature, color='tab:green')

axs[0].set_xlim([0, 0.005])
axs[0].set_ylim([-1, 1])

axs[1].set_xlim([0, 0.005])
axs[1].set_ylim([-1, 1])

plt.grid()
plt.show()
