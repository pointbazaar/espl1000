FROM ubuntu:latest

LABEL maintainer="ddockre" description="test" version="0.0.1"

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Berlin
RUN apt-get update -y && apt-get install -y tzdata

RUN apt-get install -y --no-install-recommends \
	git \
	curl \
	gcc \
	make \
	valgrind \
	unzip \
	tar \
	ca-certificates && \
    apt-get autoclean && \
    apt-get autoremove && \
    apt-get clean
