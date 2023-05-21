# PROJET: BLOCKCHAIN APPLIQUEE A UN PROCESSUS ELECTORAL
Consideration: Organisation d'un processus electoral par scrutin uninominal majoritaire a 2 tours
## Processus
1. [Implementation d'outils de cryptographie](desc/p1.md)
2. [Creation d'un systeme de declarations securises par chiffrement asymetrique]()
3. [Manipulation d'une base centralisee de declarations]()
4. [Implementation d'un mecanisme de consensus]()
5. [Manipulation d'une base decentralisee de declarations]()
## Cadre du projet
**Situation**: Problematique de designation du vainqueur d'un processus electoral
### Dans le processus electoral
- Chaque participant peut declarer sa candidature au scrutin *et/ou* donner sa voix a un candidat declare
- Depuis toujours, la tenue d'un processus electoral a pose des questions de confiance et de transparence epineuses
  - Les elections sont generalement organisees par le systeme executif en place, souvent candidat a sa reelection et donc soupconne d'interferences
  - Le compte des voix fait l'appel a des assesseurs, ce qui en fait un travail long et avec peu de garanties de fiabilite, dans la mesure ou tout le monde ne peut pas verifier que le compte a eu lieu dans des conditions honnetes
  - La caractere anonyme de la designation par bulletin fait que personne ne peut verifier a posteriori que sa voix a ete comptabilisee chez le bon candidat

- Autre aspect a considerer: Ergonomie pour le votant
  - Un systeme decentralise permettait un vote a distance, qui: 
    - a longtemps ete envisage comme un outil pour combattre l'abstension
    - a atteint un record historique lors des elections regionales et departementales 
  
**Objectif**: proposer une piste de reflexion sur les protocoles et sur les structures de donnees a mettre en place pour permettre d'implementer efficacement le processus de designation du vainqueur de l'election, tout en garantissant l'integrite, la securite et la transparence de l'election.
