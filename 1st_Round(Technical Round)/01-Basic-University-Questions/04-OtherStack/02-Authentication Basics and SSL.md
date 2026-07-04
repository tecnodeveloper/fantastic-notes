# Authentication Basics

---

## 1. Definition

**Authentication** is the process of verifying the identity of a user or system.  
**Authorization** is the process of determining what an authenticated user is allowed to do.

> Mental Model: Authentication = “Who are you?”  
> Authorization = “What can you access?”

---

## 2. Why Authentication Exists

APIs, websites, and apps need to:

- Protect sensitive data
- Ensure users are who they claim to be
- Control access to resources

Without authentication:

- Anyone could access private data
- Systems are vulnerable to misuse
- Auditing and accountability are impossible

---

## 3. Common Methods

### 3.1 Username/Password

- Simplest method  
- Server stores credentials (ideally hashed)  
- Verified during login

### 3.2 OAuth2

- Delegated authorization framework  
- Third-party apps can access resources without sharing passwords  
- Common for social logins (Google, Facebook)

### 3.3 JWT (JSON Web Token)

- Token-based authentication  
- Contains user info and expiration  
- Signed by server to prevent tampering  

### 3.4 SSO (Single Sign-On)

- Allows one login across multiple applications  
- Centralized authentication provider  
- Reduces password fatigue and improves security

---

## 4. Session-Based vs Token-Based Authentication

| Feature                  | Session-Based                       | Token-Based (JWT)                 |
|---------------------------|------------------------------------|----------------------------------|
| Storage                  | Server memory or database           | Client-side (localStorage / cookies) |
| Scalability              | Limited (server stores sessions)    | Highly scalable (stateless)      |
| Expiration               | Usually fixed                      | Configurable in token payload    |
| Cross-domain support     | Harder                             | Easy                             |
| Security                 | Susceptible to CSRF                 | Susceptible to XSS if not careful |

---

## 5. Syntax and Basic Examples

### 5.1 Username/Password Example

```javascript
// Incorrect Example: Storing plain passwords
const users = [
  { username: "alice", password: "12345" }
];

function login(username, password) {
  const user = users.find(u => u.username === username);
  if (user && user.password === password) {
    return "Login successful";
  }
  return "Invalid credentials";
}


**Fix: Use hashing**

javascript

import bcrypt from "bcryptjs";

const users = [];

async function register(username, password) {
  const hashed = await bcrypt.hash(password, 10);
  users.push({ username, password: hashed });
}

async function login(username, password) {
  const user = users.find(u => u.username === username);
  if (user && await bcrypt.compare(password, user.password)) {
    return "Login successful";
  }
  return "Invalid credentials";
}
```

---

### 5.2 Token-Based Authentication (JWT)

```javascript
import jwt from "jsonwebtoken";

const SECRET_KEY = "supersecret";

// Login endpoint
function login(user) {
  const token = jwt.sign({ id: user.id, role: user.role }, SECRET_KEY, { expiresIn: "1h" });
  return token;
}

// Middleware to protect routes
function authenticate(req, res, next) {
  const token = req.headers["authorization"]?.split(" ")[1];
  if (!token) return res.status(401).send("No token provided");
  
  try {
    const decoded = jwt.verify(token, SECRET_KEY);
    req.user = decoded;
    next();
  } catch (err) {
    return res.status(401).send("Invalid token");
  }
}
```

**Step-by-Step Breakdown**

1. User logs in → server creates a JWT
    
2. JWT contains user ID, role, and expiration
    
3. Client stores JWT in localStorage or cookie
    
4. Subsequent requests include JWT in `Authorization` header
    
5. Server verifies token before allowing access
    

---

### 5.3 Session-Based Authentication Example

```javascript
import express from "express";
import session from "express-session";

const app = express();

app.use(session({
  secret: "mysecret",
  resave: false,
  saveUninitialized: true
}));

app.post("/login", (req, res) => {
  // Assume user verified
  req.session.userId = 123;
  res.send("Logged in");
});

app.get("/profile", (req, res) => {
  if (!req.session.userId) return res.status(401).send("Not logged in");
  res.send(`User ID: ${req.session.userId}`);
});
```

---

## 6. Internal Behavior

- **Session-Based**
    
    - Server stores session ID → memory or database
        
    - Client receives session cookie
        
    - Each request checked against server session store
        
- **Token-Based**
    
    - Server signs JWT → stateless
        
    - Client stores token → sent in headers
        
    - Server validates signature and expiration
        

---

## 7. Real-World Usage

### 7.1 Backend: REST API Protection

```javascript
app.get("/dashboard", authenticate, (req, res) => {
  res.send(`Welcome User ${req.user.id}`);
});
```

### 7.2 Frontend: Storing JWT

```javascript
// After login
localStorage.setItem("token", token);

// Sending requests
fetch("/dashboard", {
  headers: {
    "Authorization": `Bearer ${localStorage.getItem("token")}`
  }
});
```

---

## 8. Edge Cases and Pitfalls

- **JWT not expired properly** → users may stay logged in longer than expected
    
- **Storing JWT in localStorage** → XSS vulnerability
    
- **Session hijacking** → need secure cookies and HTTPS
    
- **Incorrect password handling** → never store plaintext passwords
    

---

## 9. Key Points

- Authentication = verifying identity
    
- Authorization = controlling access
    
- Session vs Token → trade-offs between statefulness and scalability
    
- Always hash passwords and secure tokens
    

---

## 10. Common Mistakes (With Fixes)

|Mistake|Problem|Fix|
|---|---|---|
|Storing passwords in plaintext|Security breach|Use bcrypt hashing|
|Storing JWT in localStorage|Vulnerable to XSS|Use HTTP-only secure cookies|
|No token expiration|Permanent access if token leaked|Always set expiry and rotate tokens|
|Not validating JWT|Unauthorized access|Always verify signature and claims|

---

## 11. Interview Tips / Summary

- **What is the difference between authentication and authorization?**  
    → Authentication = identity verification, Authorization = permission control
    
- **JWT vs Session**  
    → JWT: stateless, scalable, sent in headers  
    → Session: stateful, stored on server, relies on cookies
    
- **Common security practices**  
    → Hash passwords, use HTTPS, set token expiration, secure cookies
    

---

## 12. Final Summary

- Authentication ensures only legitimate users access resources
    
- Choose method based on scale, security, and architecture
    
- Combine sessions, tokens, or SSO as needed for real-world applications
    
- Always consider edge cases and best security practicese


## http vs https
## How SSL works 