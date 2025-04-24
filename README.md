
# Movie Manager CLI

A simple command-line movie manager written in C with secure user authentication. Each user can log in or register and manage their own personal list of movies, saved to a file under their username. All passwords are hashed using SHA-256 with salt for security.

## Features

- **User Authentication**
  - Secure login and registration
  - SHA-256 password hashing with random salt
  - Credentials stored in `user_credentials.dat`

- **Movie Management**
  - Add a movie (title, genre, year, rating, review)
  - List all movies
  - Save movies to a user-specific file
  - Search movies by title or genre
  - Filter movies by minimum rating

- **File Persistence**
  - Movie lists saved to files named `<username>_movies.txt`

## Project Structure

```
.
├── auth.c          # Handles registration, login, hashing, and storage
├── file_io.c       # Handles loading/saving movie lists to files
├── main.c          # Application entry point and menu loop
├── movie.c         # Add, list, search, and filter movie functionality
├── sha256.c        # SHA-256 hashing implementation (open source)
├── ui.c            # Input handling and menu UI
├── user_credentials.dat  # Auto-created on first run to store credentials
```

## How to Compile

```bash
cd CYS445_project/src
gcc -I../include *.c -o MovieManager
```

Or can use CMAKE
```bash
cd CYS455_project/build
cmake ..
make
```

This project uses only standard C libraries, no external dependencies.

## How to Use

1. Run the program:
   ```bash
   ./MovieManager
   ```

2. Enter your username and password.
   - If the user is new, you will be prompted to register.

3. Use the menu options to:
   - Add a movie
   - List your movies
   - Save movies to a file
   - Search by title or genre
   - Filter by rating
   - Exit the program

## Security

- Passwords are salted and hashed using the SHA-256 algorithm.
- Salted hashes are stored in the format: `username:salt:hashed_password`
- No plaintext passwords are ever saved.

## License

The SHA-256 implementation is based on Brad Conte's public domain code, available at: https://github.com/B-Con/crypto-algorithms
