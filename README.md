# Lab10

Analisando os códigos desse Laboratório podemos ver que as variáveis em_e e em_l são usadas para exclusão mútua. Sendo assim, elas precisam ser iniciadas com 1 para garantir que apenas 1 thread possa realizar o mesmo bloco de código.

Além disso, as variáveis escr e leit são usadas como referência para controlar quantos escritores e leitores estão atuando ao mesmo tempo. Podemos concluir que elas também precisam ser iniciadas com 1. Isso garante que apenas 1 escritor possa escrever por vez, mas também garante que vários leitores possam ler ao mesmo tempo (com a condição de não haver escritores lendo).
