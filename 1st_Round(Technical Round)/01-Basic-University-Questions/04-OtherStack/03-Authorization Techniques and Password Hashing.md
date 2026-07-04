## 1. Definition  
  
**Authorization and Password Security** are two essential parts of application security.  
  
- **Authorization** determines what an authenticated user is allowed to access or perform.  
- **Password Security** ensures user credentials are stored and verified securely to prevent unauthorized access.  
  
> **Mental Model**  
>  
> Authentication answers **"Who are you?"**  
>  
> Authorization answers **"What are you allowed to do?"**  
>  
> Password Security answers **"How do we protect your identity?"**  
  
Together, these concepts ensure that:  
  
- Only legitimate users can log in.  
- Passwords remain protected even if the database is compromised.  
- Authenticated users can access only the resources they are permitted to use.  
  
---  
  
# 2. Why These Concepts Exist  
  
Without proper authorization and password security:  
  
- Attackers can steal user credentials.  
- Sensitive information may become accessible.  
- Unauthorized users can perform restricted actions.  
- Applications become vulnerable to privilege escalation and data breaches.  
  
These techniques provide:  
  
- Secure user authentication  
- Controlled access to resources  
- Protection against password theft  
- Compliance with security standards  
- Better overall system security  
  
---  
  
# 3. Password Security  
  
Password security focuses on protecting user credentials before and after authentication.  
  
---  
  
## 3.1 Password Hashing  
  
### What is Hashing?  
  
Hashing converts a password into a fixed-length irreversible value.  
  
Instead of storing:  
  
```  
password123  
```  
  
The database stores something similar to:  
  
```  
$2b$10$Qy9...  
```  
  
Even if attackers steal the database, they cannot directly recover the original password.  
  
### Example  
  
```javascript  
import bcrypt from "bcryptjs";  
  
async function hashPassword(password) {  
return await bcrypt.hash(password, 10);  
}  
  
async function verifyPassword(password, hash) {  
return await bcrypt.compare(password, hash);  
}  
```  
  
---  
  
## 3.2 Salting  
  
A **salt** is a random value added before hashing.  
  
Without salt:  
  
```  
password123  
↓  
  
HASH  
```  
  
With salt:  
  
```  
Random Salt  
+  
password123  
↓  
HASH  
```  
  
### Why Salt?  
  
Without salts:  
  
Two users with the same password produce identical hashes.  
  
With salts:  
  
Each password generates a completely different hash.  
  
```javascript  
const saltRounds = 12;  
  
const hashedPassword = await bcrypt.hash("password123", saltRounds);  
```  
  
---  
  
## 3.3 Secure Password Storage  
  
Never store passwords like this:  
  
```javascript  
// ❌ Wrong  
{  
username: "john",  
password: "password123"  
}  
```  
  
Always store:  
  
```javascript  
// ✅ Correct  
{  
username: "john",  
password:  
"$2b$12$Ljd8kL..."  
}  
```  
  
---  
  
## Internal Behavior of bcrypt  
  
When hashing:  
  
```  
Password  
↓  
Generate Salt  
↓  
Combine Password + Salt  
↓  
Run Hash Algorithm  
↓  
Store Final Hash  
```  
  
When logging in:  
  
```  
Entered Password  
↓  
Hash Again Using Stored Salt  
↓  
Compare Both Hashes  
↓  
Match?  
```  
  
---  
  
## Mental Model  
  
Imagine putting a document into a paper shredder.  
  
Hashing is similar.  
  
You can shred a paper, but you cannot reconstruct the original perfectly.  
  
---  
  
# 4. Authorization  
  
Authorization determines whether a user can perform a particular action after they have successfully authenticated.  
  
---  
  
## Common Authorization Techniques  
  
### 4.1 Role-Based Access Control (RBAC)  
  
Users receive predefined roles.  
  
Example:  
  
| Role | Permissions |  
|------|-------------|  
| Admin | Create, Read, Update, Delete |  
| Editor | Read, Update |  
| Viewer | Read |  
  
```javascript  
const roles = {  
admin: ["create", "read", "update", "delete"],  
editor: ["read", "update"],  
viewer: ["read"]  
};  
  
function canPerform(role, action) {  
return roles[role]?.includes(action);  
}  
  
console.log(canPerform("admin", "delete"));  
```  
  
---  
  
### Step-by-Step  
  
```  
User logs in  
↓  
Role = Admin  
↓  
Requests Delete  
↓  
Role Lookup  
↓  
Permission Found  
↓  
Access Granted  
```  
  
---  
  
## 4.2 Attribute-Based Access Control (ABAC)  
  
Instead of roles, permissions depend on attributes.  
  
Example:  
  
- Department  
- Location  
- Time  
- Resource owner  
  
```javascript  
const user = {  
department: "Sales"  
};  
  
const document = {  
department: "Sales"  
};  
  
function canEdit(user, document) {  
return user.department === document.department;  
}  
```  
  
---  
  
## 4.3 Permission-Based Authorization  
  
Instead of roles, each user owns individual permissions.  
  
```javascript  
const permissions = {  
alice: ["read", "write"],  
bob: ["read"]  
};  
  
function hasPermission(user, permission) {  
return permissions[user]?.includes(permission);  
}  
```  
  
---  
  
# 5. Authorization Flow  
  
```  
User Login  
↓  
Password Verified  
↓  
Authentication Successful  
↓  
User Requests Resource  
↓  
Authorization Check  
↓  
Allowed?  
↓  
Yes → Continue  
  
No → 403 Forbidden  
```  
  
---  
  
# 6. Backend Example  
  
Protecting an Express endpoint.  
  
```javascript  
function authorize(permission) {  
  
return (req, res, next) => {  
  
if (!canPerform(req.user.role, permission)) {  
return res.status(403).send("Forbidden");  
}  
  
next();  
};  
}  
  
app.delete("/users/:id", authorize("delete"), (req, res) => {  
  
});  
```  
  
---  
  
# 7. Frontend Example  
  
Showing UI based on permissions.  
  
```javascript  
const role = "viewer";  
  
if (canPerform(role, "update")) {  
console.log("Show Edit Button");  
} else {  
console.log("Hide Edit Button");  
}  
```  
  
---  
  
# 8. Comparison  
  
| Feature | Password Security | Authorization |  
|----------|-------------------|--------------|  
| Purpose | Protect passwords | Control resource access |  
| Used During | Login | After login |  
| Main Concern | Identity protection | Permission management |  
| Common Techniques | Hashing, Salting | RBAC, ABAC, Permissions |  
| Result | Secure authentication | Secure access control |  
  
---  
  
# 9. Real-World Usage  
  
## Backend  
  
A banking application:  
  
1. User logs in.  
2. Password verified using bcrypt.  
3. JWT issued.  
4. Authorization middleware checks whether user can transfer money.  
5. Request succeeds only if permission exists.  
  
---  
  
## Frontend  
  
An admin dashboard.  
  
Different users see different UI.  
  
```javascript  
if (user.role === "admin") {  
showDeleteButton();  
}  
```  
  
Editors cannot delete records.  
  
Viewers cannot edit records.  
  
---  
  
# 10. Edge Cases and Pitfalls  
  
## Password Security  
  
### Storing Plain Passwords  
  
 Wrong  
  
```javascript  
password: "123456"  
```  
  
✅ Correct  
  
```javascript  
password:  
"$2b$12$..."  
```  
  
---  
  
### Weak Passwords  
  
Problem  
  
```  
123456  
password  
qwerty  
```  
  
Fix  
  
Require:  
  
- Minimum length  
- Numbers  
- Symbols  
- Uppercase letters  
  
---  
  
### Using Fast Hash Algorithms  
  
```javascript  
SHA-256  
```  
  
Not recommended for passwords.  
  
Use:  
  
- bcrypt  
- Argon2  
- scrypt  
  
---  
  
## Authorization  
  
### Only Protecting the Frontend  
  
Hide the Delete button.  
  
Attackers call the API directly.  
  
Always verify permissions on the backend.  
  
---  
  
### Role Explosion  
  
Too many roles become difficult to maintain.  
  
Instead:  
  
- RBAC  
- +  
- Permission system  
  
---  
  
### Conflicting Rules  
  
ABAC rules may conflict.  
  
Define a clear policy order.  
  
---  
  
# 11. Common Mistakes  
  
| Mistake | Problem | Fix |  
|----------|---------|-----|  
| Store plain passwords | Database leak exposes passwords | Hash passwords |  
| No salting | Rainbow table attacks | Use bcrypt |  
| Using SHA-256 | Too fast for passwords | Use bcrypt/Argon2 |  
| Only frontend authorization | API bypass | Backend authorization |  
| Too many roles | Difficult management | Combine RBAC with permissions |  
| Missing permission checks | Unauthorized access | Verify every protected action |  
  
---  
  
# 12. Key Points  
  
- Hash passwords before storing them.  
- Never store plain text passwords.  
- bcrypt automatically generates salts.  
- Authentication identifies users.  
- Authorization controls access.  
- RBAC is role-based.  
- ABAC is attribute-based.  
- Always enforce authorization on the backend.  
- Password security and authorization work together.  
  
---  
  
# 13. Interview Summary  
  
### Difference between Authentication and Authorization  
  
| Authentication | Authorization |  
|---------------|--------------|  
| Verifies identity | Verifies permissions |  
| Happens first | Happens after authentication |  
| Login | Resource access |  
  
---  
  
### Why hash passwords?  
  
- Prevent password theft  
- Protect leaked databases  
- One-way conversion  
  
---  
  
### Why use bcrypt?  
  
- Slow hashing algorithm  
- Built-in salt generation  
- Resistant to brute-force attacks  
  
---  
  
### RBAC vs ABAC  
  
| RBAC | ABAC |  
|------|------|  
| Based on roles | Based on attributes |  
| Easier | More flexible |  
| Static | Dynamic |  
  
---  
  
### Best Practices  
  
- Never store plain passwords.  
- Always hash and salt passwords.  
- Use bcrypt or Argon2.  
- Enforce authorization on every protected endpoint.  
- Never trust the client.  
- Use the principle of least privilege.  
  
---  
  
# 14. Final Summary  
  
Password security and authorization are fundamental pillars of application security.  
  
Password security protects user credentials by hashing and salting passwords before storage, making them resistant to theft and brute-force attacks.  
  
Authorization ensures authenticated users can access only the resources and actions they are permitted to use through techniques such as Role-Based Access Control (RBAC), Attribute-Based Access Control (ABAC), and permission-based checks.  
  
Together, these practices provide a secure authentication process, prevent unauthorized access, protect sensitive data, and form the foundation of modern web application security.