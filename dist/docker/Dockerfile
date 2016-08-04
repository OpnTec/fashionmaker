FROM gcc
MAINTAINER Roman Telezhynskyi <dismine@gmail.com>

RUN apt-get update && apt-get install -y \
    cppcheck \
    locales \
    xvfb \
    qtbase5-dev \
    libqt5svg5-dev \
    qt5-default \
    qttools5-dev-tools \
    libqt5xmlpatterns5-dev \
    libqt5core5a \
    libqt5gui5 \
    libqt5printsupport5 \
    libqt5svg5 \
    libqt5widgets5 \
    libqt5xml5 \
    libqt5xmlpatterns5 \
    xpdf \
    && rm -rf /var/lib/apt/lists/*

# Whatever you need more than what is on the base image required by your project
 
# Set the locale
RUN dpkg-reconfigure locales && \
    locale-gen en_US.UTF-8

ENV LANG en_US.UTF-8  
ENV LANGUAGE en_US:en  
ENV LC_ALL en_US.UTF-8
