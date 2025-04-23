feito por Gabriel Brum

# 📌 Guia de Git do Deepseek

🔧 Configuração Inicial
```bash
git config --global user.name "Seu Nome"
git config --global user.email "seu@email.com"
git config --global core.editor "code --wait"  # Opcional (VS Code)
```
---

📥 Clonar o Repositório
```bash
git clone <URL-do-repositório>
```

--- 

🌿 Branching 
```bash
git branch                      # Lista branches
git branch <nome-da-branch>     # Cria nova branch
git checkout <nome-da-branch>   # Muda para a branch
git checkout -b <nova-branch>   # Cria e muda para a branch
```

---

🔄 Sincronização com o Remoto
```bash
git fetch                       # Busca atualizações sem modificar código
git pull origin <branch>        # Puxa alterações e faz merge
git push origin <sua-branch>    # Envia alterações para o remoto
```

---

💾 Commits
```bash
git add <arquivo> ou git add .  # Prepara arquivos para commit (o git add . faz com que todos os arquivos modificados sejam adicionados ao commit)
git commit -m "Mensagem clara"  # Registra alterações
git commit --amend              # Corrige último commit (local)
```

---

⚠️ Resolução de Conflitos
```bash
git diff                        # Mostra diferenças
git merge <branch>              # Mescla branches
```

---

🤓 Resumo prático 

```bash

git fetch origin                  # Verifica atualizações remotas
                                    # se tiver alteração, faz um 'git pull origin nome-da-branch'
git status                        # Confere mudanças locais
git add .                         # Adiciona tudo que foi modificado ao stage (se n quiser add tudo é só ir add um por um 
                                    # OU add tudo e utiliza o 'git restore --staged' para tirar apenas os que não deseja)
                              
git commit -m "Mensagem clara"    # Cria o commit
git push origin nome-da-branch    # Envia para o remoto

```
