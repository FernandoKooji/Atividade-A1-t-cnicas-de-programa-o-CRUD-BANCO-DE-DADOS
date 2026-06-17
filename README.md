# Atividade A1 técnicas de programação (CRUD / GERENCIADOR DE LOG DE E-MAILS)

Sistema desenvolvido em C para gerenciamento de registros de e-mails utilizando lista encadeada dinâmica, validação de dados e persistência em arquivos texto e binários.

## Funcionalidades

## Menu Principal:

<img width="500" alt="menu" src="https://github.com/user-attachments/assets/9ffdae1d-e2d4-4d82-b0ae-02c3e0cd00da" />

## Importação de Arquivos:

<img width="500"  alt="menu importação" src="https://github.com/user-attachments/assets/0fce7495-e2cd-4282-bca8-3d3cc3b7c9ed" />

## Listar Registros:

<img width="523"  alt="registros listados" src="https://github.com/user-attachments/assets/17c08aeb-0464-4358-8acd-8f32a9ef2acc" />

Exibe todos os registros armazenados na lista encadeada juntamente com seus respectivos IDs.

## Adicionar Registros:

<img width="500"  alt="novo registro" src="https://github.com/user-attachments/assets/f7289f3c-81e2-458d-b13f-350fe4d359b6" />

Cadastra um novo registro contendo nome, e-mail, data e horário, realizando validações antes da inserção na lista.

<img width="500"  alt="validação" src="https://github.com/user-attachments/assets/3abd87b4-bf4b-46fb-8f74-4326d9ae0226" />

Exemplo de email inválido

<img width="500"  alt="listar registros novo" src="https://github.com/user-attachments/assets/fd20b3b2-4701-41c4-b92e-781d20ffb8ad" />

Novo registro adicionado

## Remover Registro:

<img width="500"  alt="remover registro" src="https://github.com/user-attachments/assets/e8ad0f01-1ac9-425b-8c83-244eda141c3d" />

Remove um registro da lista utilizando seu identificador único, reorganizando os ponteiros da estrutura.

## Busca por nome ou data:

<img width="500"  alt="busca nome ou data" src="https://github.com/user-attachments/assets/21862109-f30d-4767-b739-645b70f1e3ad" />

Localiza registros através de uma busca textual, permitindo encontrar nomes completos ou parciais sem diferenciar letras maiúsculas e minúsculas.
Filtra e exibe todos os registros associados a uma data específica.

## Exportação de Arquivos:

<img width="500"  alt="exportar bin" src="https://github.com/user-attachments/assets/402f3d35-3502-494c-ae09-36ec60e3fb77" />

## Conceitos Aplicados

- Lista Encadeada

	A estrutura utiliza nós conectados por ponteiros, permitindo inserção e remoção dinâmica de registros.

- Alocação Dinâmica

	Os registros são criados utilizando malloc() e liberados com free().

- Persistência

	O sistema realiza leitura e escrita de arquivos texto e binários para armazenamento permanente dos dados.

## Autores

Fernando Shimomura, Gabriel Benjamin e Victor Ferreira

Projeto acadêmico desenvolvido para a disciplina de Estruturas de Dados / Técnicas de Programação.
