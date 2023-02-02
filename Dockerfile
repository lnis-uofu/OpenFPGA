FROM ghcr.io/lnis-uofu/openfpga-master:latest

# Install node js
USER root
RUN curl -sL https://deb.nodesource.com/setup_14.x | bash -
RUN curl -fsSL https://code-server.dev/install.sh | sh
RUN apt-get install -y nodejs
RUN apt-get install tree
RUN code-server --install-extension ms-python.python

ARG NB_USER=openfpga_user
ARG NB_UID=1000
ENV USER ${NB_USER}
ENV NB_UID ${NB_UID}
ENV HOME /home/${NB_USER}

RUN chown -R ${NB_UID} ${HOME}
RUN chown -R ${NB_UID} /opt/openfpga
USER ${NB_USER}

ENV PATH $PATH:/home/${NB_USER}/.local/bin

RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install --user --no-cache-dir notebook
RUN python3 -m pip install --user --no-cache-dir jupyterlab
RUN python3 -m pip install --user --no-cache-dir jupyterhub
RUN python3 -m pip install --user --no-cache-dir jupyter-server-proxy
RUN python3 -m pip install --user --no-cache-dir jupyter-vscode-proxy

RUN npm install @jupyterlab/server-proxy
RUN jupyter serverextension enable --py jupyter_server_proxy
RUN jupyter labextension install @jupyterlab/server-proxy
RUN jupyter lab build
RUN git reset --hard HEAD

# Set up terminal
RUN echo 'export PS1="\[$(tput bold)\]\[\033[38;5;220m\]\u\[$(tput sgr0)\]:\[$(tput sgr0)\]\[$(tput bold)\]\[\033[38;5;14m\]\W\[$(tput sgr0)\]\\$\[$(tput sgr0) \]"' >> ~/.bashrc
RUN echo 'alias codeopen="code-server -r "' >> ~/.bashrc
RUN mkdir -p .vscode && echo '{"files.associations": {"*.openfpga": "shellscript"},"workbench.colorTheme": "Monokai"}' > .vscode/settings.json
