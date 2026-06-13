# radioactive-material-analysis

A command-line application for storing and managing radioactive material data in C.

---

## About

Written in C, this program manages a registry of radioactive materials, each with a name, mass number, and half-life in seconds. Data is loaded from and saved to a text file. The application supports registering new materials, listing, sorting, searching, editing, and deleting entries through an interactive menu.

## Usage

Run the program and enter a filename to load existing data from (or a new filename to start fresh). Enter `def` to use the default file `RadioactiveMaterialData.txt`. Follow the on-screen menu to manage the data. Half-life values can be entered in seconds (s), minutes (m), hours (h), days (d), or years (y) - for example `10e+18 h`.

A sample data file is included at `src/RadioactiveMaterialData.txt`.

## Getting Started

### Prerequisites

- A C compiler (gcc or equivalent)

### Building

**Unix**
```
gcc -o radioactive-material-analysis src/RadioactiveMaterialAnalysis.c
```

**Windows**
```
gcc -o radioactive-material-analysis.exe src\RadioactiveMaterialAnalysis.c
```

### Running

**Unix**
```
./radioactive-material-analysis
```

**Windows**
```
radioactive-material-analysis.exe
```

## Configuration

| Constant | Default | Description |
|----------|---------|-------------|
| `MAXREGISTERS` | 1000 | Maximum number of materials that can be stored |
| `MAXNAMELENGTH` | 20 | Maximum characters in a material name |
| `DEFAULTFILENAME` | `"RadioactiveMaterialData.txt"` | Default data file name |
| `MAXFILENAMELENGTH` | 500 | Maximum length of the user-provided filename |

---

MIT License - see [LICENSE](LICENSE)
