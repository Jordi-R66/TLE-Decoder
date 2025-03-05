# Résumé des recherches sur le calcul des coordonnées 3D d'un point sur une orbite

## Rappel des variables

### Paramètres orbitaux

- $i$ : inclinaison de l'orbite
- $\Omega$ : longitude du nœud ascendant
- $e$ : excentricité de l'orbite
- $\omega$ : argument du péricentre
- $T$ : date de passage au péricentre
- $a$ : demi-grand axe
- $\nu$ : anomalie vraie

On peut remplacer $\omega$ par $\overline \omega = \Omega + \omega$ (la longitude du péricentre).

## Calcul des coordonnées 3D dans un système inertiel

### Prérequis

Avant de procéder au calcul des coordonnées 3D, il est nécessaire de connaître les coordonnées polaires du point

- $r$ : distance du point au foyer de l'ellipse
- $\vartheta$ : latitude
- $\varphi$ : longitude

### Calcul des coordonnées 3D

$$
x = r \sin \vartheta \cos \varphi \\
y = r \sin \vartheta \sin \varphi \\
z = r \cos \vartheta
$$

### Calcul des coordonnées polaires

#### Calcul de $r$

$$
r = \frac{a(1 - e^2)}{1 + e \cos f}
$$

#### Calcul de $\vartheta$

$$
\sin \vartheta = \sin i \sin f
$$

$$
\cos \vartheta = \cos i \cos f
$$

#### Calcul de $\varphi$


## Donné par Mistral AI

$$
r = \dfrac{a(1 - e^2)}{1 + e \cos(\nu)} \\
x' = r \cos(\nu) \\
y' = r \sin(\nu) \\
x = x' (\cos(\Omega) \cos(\omega) - \sin(\Omega) \sin(\omega) \cos(i)) - y' (\cos(\Omega) \sin(\omega) + \sin(\Omega) \cos(\omega) \cos(i)) \\
y = x' (\sin(\Omega) \cos(\omega) + \cos(\Omega) \sin(\omega) \cos(i)) + y' (\cos(\Omega) \cos(\omega) \sin(\Omega) - \sin(\Omega) \sin(\omega) \cos(i)) \\
z = x' \sin(\omega) \sin(i) + y' \cos(\omega) \sin(i)
$$

