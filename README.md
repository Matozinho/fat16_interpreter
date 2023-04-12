# Fat 16 Interpreter

A interpreter for Fat 16 images.

## Usage

You can run it with [xmake](https://xmake.io/) or with [docker](https://www.docker.com/).

To run with xmake:

```bash
xmake && xmake r interpreter <path-to-img>
```

To run with docker:

- Build the image and create the container with the script (this will open container after create)
```bash
./scripts/create_container
```
- To run the app, you can use the script:
```bash
./scripts/run <path-to-img>
```
- To delete the image and the container after usage, you can run:
```bash
./scripts/delete_container
```
- To use another image, you have 3 options: 
  1) Move the image to the `assets` folder before create the docker image;
  2) Copy the object from the local machine to the `assets` folder and re-build the project with the following command:
  ```bash
  # in the local machine
  docker cp <path-to-img-in-local-machine> matozinho_fat16:/home/cppdev/assets
  ```
  ```bash
  # in the container
  xmake build -a
  ```
  3) Copy the image directly to the folder with the executable
  ```bash
  # in the local machine
  docker cp <path-to-img-in-local-machine> matozinho_fat16:/home/cppdev/build/linux/x86_64/release/assets
  ```
  
### Useful commands for docker
```bash
# if the container is stopped, start and connect to it
docker start matozinho_fat16 && docker attach matozinho_fat16
```
```bash
# list all running containers
docker ps
```

## Interpreter

The interpreter starts on the root dir of the image, and you can use the command `help` to see all available commands.
```bash
[cppdev@12283e978376 ~]$ ./scripts/run assets/fat16_4sectorpercluster.img
|> help
Available commands:
+---------+----------------------------------------+----------------+
| Command | Description                            | Arguments      |
+---------+----------------------------------------+----------------+
| clear   | Clear the console                      | ()>            |
+---------+----------------------------------------+----------------+
| pwd     | Show the path to the current directory | ()>            |
+---------+----------------------------------------+----------------+
| cd      | Change to the specified directory      | (std::string)> |
+---------+----------------------------------------+----------------+
| cat     | Show the content of the specified file | (std::string)> |
+---------+----------------------------------------+----------------+
| ls      | List all the entries of the directory  | ()>            |
+---------+----------------------------------------+----------------+
|>
```

## Examples

```bash
./scripts/create_container
```
```bash
[cppdev@9c4cc4d9c605 ~]$ ./scripts/run assets/fat16_4sectorpercluster.img
```
```bash
|> help
Available commands:
+---------+----------------------------------------+----------------+
| Command | Description                            | Arguments      |
+---------+----------------------------------------+----------------+
| clear   | Clear the console                      | ()>            |
+---------+----------------------------------------+----------------+
| pwd     | Show the path to the current directory | ()>            |
+---------+----------------------------------------+----------------+
| cd      | Change to the specified directory      | (std::string)> |
+---------+----------------------------------------+----------------+
| cat     | Show the content of the specified file | (std::string)> |
+---------+----------------------------------------+----------------+
| ls      | List all the entries of the directory  | ()>            |
+---------+----------------------------------------+----------------+
|>
```
