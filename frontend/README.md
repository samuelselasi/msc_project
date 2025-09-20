# HEKSM Frontend Interface

This is the frontend interface for the **Homomorphic Encryption-based Keyword Search Mechanism (HEKSM)** project. 
It provides a simple UI for performing keyword searches, visualizing results, and viewing audit logs.

---

## Installation

Before you begin, ensure you have **Node.js (>= 16)** and **npm** installed.

Clone the main repository and navigate to the frontend directory:

```
git clone https://github.com/samuelselasi/heksm.git
cd heksm/frontend
```

Install the project dependencies:

```
nvm use 20
npm install
```

---

## Running the Development Server

Start the local dev server:

```
npm run dev
```

---

## Project Structure

```
frontend/
├── public/              # Static files
├── src/                 # Source code (components, pages, services)
│   ├── components/      # UI components
│   ├── pages/           # Main views or routes
│   └── App.jsx          # Root component
├── package.json
├── vite.config.js       # Or similar if using Vite
└── README.md            # This file

```

---

## API Integration

The frontend communicates with the backend keyword search service via REST endpoints. 
Make sure the backend server is running and update any base URLs if needed in:

```
src/services/api.js
```

---

## Troubleshooting

* Port conflict? Change the port in `.env` or `vite.config.js`
* Backend down? Ensure your `C++`/`Python` services are running locally before querying.

