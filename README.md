# Overview

A very symplistic implementation of pub/sub pattern using only ANSI C and
Redis.

# Pre-requisites

* A C compiler, e.g., [gcc](https://gcc.gnu.org)
* [git](https://git-scm.com/): A distributed version control system
* [Redis](https://redis.io/): An in-memory data store
* [hiredis](https://github.com/redis/hiredis): A minimalistic C client library
  for the Redis database
* [Doxygen](https://www.doxygen.nl/index.html) (optional): A documentation
  generator

# Installation

## Redis

Consult offical [documentation](https://redis.io/docs/getting-started/) how to
install Redis on your platform.  In this guide we run Redis Stack in a Docker
container.  Below is an "executive summary" compiled based on information
available on
[this](https://redis.io/docs/getting-started/install-stack/docker/) website.

To get started with Redis Stack downlaod its Docker image with
```
docker pull redis/redis-stack:latest
```

Then start Redis Stack container by running the command below:
```
docker run -d --name redis-stack -p 6379:6379 -p 8001:8001 redis/redis-stack:latest
```

The `docker run` command above also exposes RedisInsight on port 8001. Point
your browser to `localhost:8001` to use it.

## hiredis

In most major GNU/Linux distributions **hiredis** is available a package which
can be easily installed using the package manager that comes with the
distribution.  For example, on Ubuntu 22.04 you can install **hiredis** with
```
sudo apt install libhiredis-dev
```

If the package with the library is not available on your platform, you can
compile it source using [cmake](https://cmake.org).

## TIM-towncrier

At the moment there is no package you can download and install. The code must
be compiled manually using `cmake`.

Clone the repository with to a directory of choice:
```
git clone
```

Compile the code with
```
cd TIM-towncrier
cmake -S . -B build
cmake --build build
```

If the compilation was successful you should see two binary files in the
`build` directory: `pub` and `sub`.

Optionally, you can place them at any location specified by you `PATH`
environment variable, e.g., `$HOME/bin`. Otherwise you will need to specify the
full path (absolute or relative) when calling any of these binaries.

# Usage

Make sure the Docker container with your Redis Stack is up and running.

Open a terminal and start the publisher with 
```
pub foo
```
where `foo` is the name of the channel the publisher will be sending its
messages to.

By default, the publisher assumes that the Redis instance is running locally on
you computer. If it is not the case, point the publisher to the host running
the Redis data store:
```
pub -H 10.0.0.2 foo
```

Type `pub -h` to see all supported options and arguments.

Once the publisher starts sending messages (consecutive integers padded with
0s) to the channel you should see them in the terminal. For example:
```
$ ./pub foo
[INFO] Connecting to the message broker
[INFO] Creating a publisher
[INFO] Starting to send messages
[INFO] Message '000000' sent, no. of subscribers: 0
[INFO] Message '000001' sent, no. of subscribers: 0
...
```

Open another terminal and similarly start the subscriber with
```
sub foo
```

If everything worked as expected you should see messages that subscriber
receives from the channel it is subscribed to. For example:

```
$ ./sub foo
[INFO] Connecting to the message broker
[INFO] Creating a subscriber
[INFO] Starting to monitor channel 'foo'
[INFO] Received message '000038' from channel 'foo'
[INFO] Received message '000039' from channel 'foo'
...
```

You can terminate both the publisher and the subscriber by pressing `Ctrl+C` in
each of the terminal you opened previously.


# Documentation

To generate the documentation from the annotated sources run following commands
```
cd docs
doxygen
```
and open `file:///path/to/TIM-towncrier/doc/html/index.html` in your browser.
