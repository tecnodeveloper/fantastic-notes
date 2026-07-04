# SQL Query Execution Order: The Big Six

## What Is SQL Query Execution Order?

SQL queries are **written in one order** but **executed in another**.

When you write a query, it usually starts with `SELECT`. However, the database engine does **not** execute `SELECT` first. Instead, it follows a logical execution order to determine:

- Which tables to read    (Is it exist)
- Which rows to keep    
- How to group the data    
- How to filter grouped data    
- Which columns to return    
- How to sort the final output    

Understanding this execution order is one of the most important SQL skills because it explains **why certain queries work and why others fail**.

---

# The Big Six Execution Order

Every standard SQL query logically executes in this order:

| Execution Order | Clause          | Purpose                                             |
| --------------- | --------------- | --------------------------------------------------- |
| 1               | `FROM` / `JOIN` | from which table you want to extract column         |
| 2               | `WHERE`         | put condition on columns                            |
| 3               | `GROUP BY`      | Create groups                                       |
| 4               | `HAVING`        | Remove unwanted groups                              |
| 5               | `SELECT`        | retrieval of data from column                       |
| 6               | `ORDER BY`      | Sort the order in acending order or decending order |

---

# Mental Model

Think of SQL as an assembly line.

```
Tables
   │
   ▼
FROM / JOIN
   │
   ▼
WHERE
   │
   ▼
GROUP BY
   │
   ▼
HAVING
   │
   ▼
SELECT
   │
   ▼
ORDER BY
   │
   ▼
Final Result
```

Every stage receives the output of the previous stage.

---

# Sample Database

Assume these tables.

## Employees

| employee_id | name    | department | salary | city   |
| ----------- | ------- | ---------- | ------ | ------ |
| 1           | Alice   | HR         | 60000  | London |
| 2           | Bob     | IT         | 90000  | Paris  |
| 3           | Charlie | IT         | 85000  | London |
| 4           | David   | Sales      | 50000  | Berlin |
| 5           | Emma    | Sales      | 70000  | London |

---

## Orders

|order_id|customer_id|amount|
|---|---|---|
|101|1|300|
|102|1|500|
|103|2|900|
|104|3|200|

---

# Step 1 — FROM / JOIN


It determines where the data comes from like table name

If multiple tables exist, SQL joins them first.

---

## Why It Exists

Without building the dataset first, SQL has nothing to filter or group.

---

## Syntax

```sql
SELECT columns
FROM table_name;
```

With joins:

```sql
SELECT columns
FROM table1
JOIN table2
ON table1.id = table2.id;
```

---

## Basic Example

```sql
SELECT *
FROM Employees;
```

Execution:

```
Read Employees table
```

---

## JOIN Example

```sql
SELECT
    o.order_id,
    c.customer_name
FROM Orders o
JOIN Customers c
ON o.customer_id = c.customer_id;
```

Execution:

```
Read Orders
↓

Read Customers

↓

Match customer_id

↓

Create combined dataset
```

---

## Internal Behavior

The optimizer chooses the most efficient join algorithm:

- Nested Loop Join
    
- Hash Join
    
- Merge Join
    

The execution plan may differ, but the **logical execution order** remains the same.

---

## Key Points

- Always first
    
- Creates working dataset
    
- Joins happen here
    
- No filtering yet
    

---

# Step 2 — WHERE

## Definition

Filters rows before grouping or calculations.

---

## Why It Exists

Processing fewer rows improves performance.

---

## Syntax

```sql
SELECT columns
FROM table
WHERE condition;
```

---

## Example

```sql
SELECT *
FROM Employees
WHERE salary > 60000;
```

Rows removed immediately:

|Name|Salary|
|---|---|
|Alice|60000|
|David|50000|

Remaining rows:

|Name|Salary|
|---|---|
|Bob|90000|
|Charlie|85000|
|Emma|70000|

---

## Incorrect Example

```sql
SELECT department,
COUNT(*)
FROM Employees
WHERE COUNT(*) > 2
GROUP BY department;
```

Error.

Why?

Because `WHERE` runs **before** grouping.

`COUNT()` doesn't exist yet.

---

## Correct Version

```sql
SELECT department,
COUNT(*)
FROM Employees
GROUP BY department
HAVING COUNT(*) > 2;
```

---

## Key Points

- Removes rows
    
- Executes before grouping
    
- Cannot use aggregate functions
    

---

# Step 3 — GROUP BY

## Definition

Groups rows having identical values.

---

## Why It Exists

To calculate summaries.

Examples:

- Total sales
    
- Average salary
    
- Number of employees
    

---

## Syntax

```sql
SELECT column,
COUNT(*)
FROM table
GROUP BY column;
```

---

## Example

```sql
SELECT
department,
COUNT(*) AS total_employees
FROM Employees
GROUP BY department;
```

Output

|Department|Employees|
|---|---|
|HR|1|
|IT|2|
|Sales|2|

---

## Internal Behavior

SQL creates buckets.

```
IT
  Bob
  Charlie

Sales
  David
  Emma

HR
  Alice
```

Aggregates operate inside each bucket.

---

## Key Points

- Groups identical values
    
- Enables aggregates
    
- Reduces multiple rows into summaries
    

---

# Step 4 — HAVING

## Definition

Filters grouped data.

---

## Why It Exists

Since `WHERE` cannot see aggregate values, SQL needs another filter.

---

## Syntax

```sql
SELECT column,
COUNT(*)
FROM table
GROUP BY column
HAVING COUNT(*) > 2;
```

---

## Example

```sql
SELECT
department,
AVG(salary)
FROM Employees
GROUP BY department
HAVING AVG(salary) > 70000;
```

Result

|Department|Average Salary|
|---|---|
|IT|87500|

---

## WHERE vs HAVING

|Feature|WHERE|HAVING|
|---|---|---|
|Runs Before Grouping|Yes|No|
|Runs After Grouping|No|Yes|
|Uses Aggregates|No|Yes|

---

## Common Mistake

Incorrect

```sql
SELECT department,
AVG(salary)
FROM Employees
WHERE AVG(salary) > 70000
GROUP BY department;
```

Correct

```sql
SELECT department,
AVG(salary)
FROM Employees
GROUP BY department
HAVING AVG(salary) > 70000;
```

---

# Step 5 — SELECT

## Definition

Chooses what appears in the final result.

---

## Why It Exists

The database now knows:

- valid rows
    
- groups
    
- aggregate values
    

Now it can safely produce output.

---

## Syntax

```sql
SELECT column1,
column2
FROM table;
```

---

## Example

```sql
SELECT
name,
salary
FROM Employees;
```

---

## Example With Aggregate

```sql
SELECT
department,
AVG(salary) AS average_salary
FROM Employees
GROUP BY department;
```

---

## Important Rule

Every selected column must either:

- appear in `GROUP BY`
    
- or be aggregated
    

Incorrect

```sql
SELECT
department,
salary
FROM Employees
GROUP BY department;
```

Correct

```sql
SELECT
department,
AVG(salary)
FROM Employees
GROUP BY department;
```

---

# Step 6 — ORDER BY

## Definition

Sorts final output.

---

## Why It Exists

Sorting should happen after the final result has been produced.

---

## Syntax

```sql
ORDER BY column ASC;
```

or

```sql
ORDER BY column DESC;
```

---

## Example

```sql
SELECT
name,
salary
FROM Employees
ORDER BY salary DESC;
```

Output

```
Bob
Charlie
Emma
Alice
David
```

---

## Multiple Columns

```sql
SELECT *
FROM Employees
ORDER BY department ASC,
salary DESC;
```

---

## Key Points

- Always last
    
- Does not change rows
    
- Only changes presentation order
    

---

# Complete Execution Walkthrough

Query

```sql
SELECT
department,
AVG(salary) AS avg_salary
FROM Employees
WHERE city = 'London'
GROUP BY department
HAVING AVG(salary) > 65000
ORDER BY avg_salary DESC;
```

---

## Step 1

```sql
FROM Employees
```

Read all rows.

---

## Step 2

```sql
WHERE city = 'London'
```

Remaining:

|Name|Department|Salary|
|---|---|---|
|Alice|HR|60000|
|Charlie|IT|85000|
|Emma|Sales|70000|

---

## Step 3

```sql
GROUP BY department
```

Groups

```
HR
  Alice

IT
  Charlie

Sales
  Emma
```

---

## Step 4

Calculate averages.

```
HR -> 60000

IT -> 85000

Sales -> 70000
```

---

## Step 5

```sql
HAVING AVG(salary) > 65000
```

Remove HR.

Remaining

```
IT
Sales
```

---

## Step 6

```sql
SELECT
department,
AVG(salary)
```

Return

|Department|Average|
|---|---|
|IT|85000|
|Sales|70000|

---

## Step 7

```sql
ORDER BY avg_salary DESC;
```

Final output

|Department|Average|
|---|---|
|IT|85000|
|Sales|70000|

---

# The Core 10%

Most production SQL goes beyond the Big Six. These concepts are essential for solving real-world problems.

---

# JOIN

## Definition

A `JOIN` combines rows from two or more tables using a related column.

It allows normalized data stored in separate tables to be queried as a single dataset.

---

## Why It Exists

Databases avoid storing duplicate information.

Instead of storing customer names inside every order, an order stores only a `customer_id`.

`JOIN` reconnects the related data.

---

## Basic Syntax

```sql
SELECT
    t1.column,
    t2.column
FROM table1 AS t1
JOIN table2 AS t2
ON t1.id = t2.id;
```

---

## INNER JOIN Example

```sql
SELECT
    c.customer_name,
    o.order_id,
    o.amount
FROM Customers AS c
JOIN Orders AS o
ON c.customer_id = o.customer_id;
```

Only customers with matching orders are returned.

---

## LEFT JOIN Example

```sql
SELECT
    c.customer_name,
    o.order_id
FROM Customers AS c
LEFT JOIN Orders AS o
ON c.customer_id = o.customer_id;
```

Returns:

- every customer
    
- matching orders if they exist
    
- `NULL` when no matching order exists
    

---

## JOIN Comparison

|JOIN Type|Returns|
|---|---|
|INNER JOIN|Matching rows only|
|LEFT JOIN|All left rows + matches|
|RIGHT JOIN|All right rows + matches|
|FULL OUTER JOIN|All rows from both tables|

---

## Triggers in SQL

Triggers in sql are special function that automatically runs when specific event occurs

---

## Common Mistake

Incorrect

```sql
SELECT *
FROM Customers
JOIN Orders;
```

No join condition.

This creates a Cartesian product.

Correct

```sql
SELECT *
FROM Customers
JOIN Orders
ON Customers.customer_id = Orders.customer_id;
```

---

## Mental Model

```
Customers
      │
customer_id
      │
      ▼
Orders
```

The matching key connects the tables.

---

# CASE WHEN

## Definition

`CASE WHEN` adds conditional logic directly inside a SQL query.

It works like an `if...else` statement.

---

## Why It Exists

Instead of processing data in application code, SQL can categorize or transform values while querying.

---

## Syntax

```sql
CASE
    WHEN condition THEN result
    WHEN condition THEN result
    ELSE result
END
```

---

## Basic Example

```sql
SELECT
    name,
    salary,
    CASE
        WHEN salary >= 80000 THEN 'High'
        WHEN salary >= 60000 THEN 'Medium'
        ELSE 'Low'
    END AS salary_level
FROM Employees;
```

---

## CASE with Aggregation

```sql
SELECT
    department,
    SUM(
        CASE
            WHEN salary >= 70000 THEN 1
            ELSE 0
        END
    ) AS high_salary_count
FROM Employees
GROUP BY department;
```

---

## Common Mistake

Incorrect

```sql
CASE salary > 50000
    THEN 'Yes'
END
```

Correct

```sql
CASE
    WHEN salary > 50000 THEN 'Yes'
    ELSE 'No'
END
```

---

## Mental Model

```
Condition 1?

Yes
 ↓

Return Value

No
 ↓

Check Next Condition

↓

ELSE
```

---

# Window Functions

Window functions calculate values across related rows **without collapsing multiple rows into a single summary**.

Unlike `GROUP BY`, every original row remains in the result.

---

## Why They Exist

Questions like these cannot be solved cleanly with `GROUP BY`:

- Highest salary in each department
    
- Second highest sale
    
- Running total
    
- Rank students
    
- Top three products per category    

---

## ROW_NUMBER()

### Definition

Assigns a unique sequential number within each partition.

---

### Syntax

```sql
ROW_NUMBER() OVER (
    PARTITION BY column
    ORDER BY column
)
```

---

### Example

```sql
SELECT
    name,
    department,
    salary,
    ROW_NUMBER() OVER (
        PARTITION BY department
        ORDER BY salary DESC
    ) AS row_num
FROM Employees;
```

Result

|Department|Name|Salary|Row Number|
|---|---|---|---|
|IT|Bob|90000|1|
|IT|Charlie|85000|2|
|Sales|Emma|70000|1|
|Sales|David|50000|2|

---

## Find Highest Paid Employee in Each Department

```sql
SELECT *
FROM (
    SELECT
        name,
        department,
        salary,
        ROW_NUMBER() OVER (
            PARTITION BY department
            ORDER BY salary DESC
        ) AS row_num
    FROM Employees
) AS ranked
WHERE row_num = 1;
```

---

## RANK()

### Definition

Ranks rows while allowing ties.

Example

```
100
100
90
```

Ranks become

```
1
1
3
```

---

### Example

```sql
SELECT
    name,
    salary,
    RANK() OVER (
        ORDER BY salary DESC
    ) AS salary_rank
FROM Employees;
```

---

## ROW_NUMBER() vs RANK()

|Feature|ROW_NUMBER()|RANK()|
|---|---|---|
|Duplicate Values|Never|Yes|
|Skips Numbers|No|Yes|
|Every Row Unique|Yes|No|

---

## Mental Model

`GROUP BY`

```
100 rows

↓

10 rows
```

Window Function

```
100 rows

↓

100 rows
+
extra calculations
```

---

# Comparison of Big Six Clauses

|Clause|Filters Rows|Filters Groups|Uses Aggregates|Changes Number of Rows|Primary Purpose|
|---|---|---|---|---|---|
|FROM|No|No|No|No|Build dataset|
|WHERE|Yes|No|No|Yes|Filter rows|
|GROUP BY|No|No|Yes|Yes|Create summaries|
|HAVING|No|Yes|Yes|Yes|Filter summaries|
|SELECT|No|No|Yes|No|Return columns|
|ORDER BY|No|No|No|No|Sort output|

---

# Real-World Use Cases

## 1. Sales Dashboard (Backend)

A backend service generates monthly revenue by region.

```sql
SELECT
    region,
    SUM(amount) AS total_sales
FROM Orders
WHERE order_date >= '2025-01-01'
GROUP BY region
ORDER BY total_sales DESC;
```

This powers dashboard widgets showing regional performance.

---

## 2. E-commerce Product Rankings

Display the best-selling product in each category.

```sql
SELECT *
FROM (
    SELECT
        product_name,
        category,
        total_sales,
        ROW_NUMBER() OVER (
            PARTITION BY category
            ORDER BY total_sales DESC
        ) AS row_num
    FROM ProductSales
) AS ranked
WHERE row_num = 1;
```

This query is commonly used by backend APIs serving storefronts.

---

## 3. Customer Loyalty Classification

Classify customers based on lifetime spending.

```sql
SELECT
    customer_id,
    SUM(amount) AS total_spent,
    CASE
        WHEN SUM(amount) >= 10000 THEN 'Platinum'
        WHEN SUM(amount) >= 5000 THEN 'Gold'
        ELSE 'Standard'
    END AS loyalty_level
FROM Orders
GROUP BY customer_id;
```

Useful for CRM systems and marketing campaigns.

---

# Common Mistakes

|Mistake|Why It Happens|Fix|
|---|---|---|
|Using `COUNT()` inside `WHERE`|Aggregates are unavailable before grouping|Use `HAVING`|
|Selecting non-grouped columns|SQL cannot determine which value to return|Add the column to `GROUP BY` or aggregate it|
|Forgetting `ON` in a `JOIN`|Produces a Cartesian product|Always specify the join condition|
|Assuming `SELECT` runs first|SQL executes clauses logically, not by written order|Learn the execution order|
|Expecting `GROUP BY` to keep all rows|Grouping summarizes rows|Use window functions when you need row-level detail|
|Using `ROW_NUMBER()` when ties should share the same rank|Unique numbering ignores ties|Use `RANK()` or `DENSE_RANK()` depending on requirements|

---

# Edge Cases and Pitfalls

## `NULL` Values in `GROUP BY`

`NULL` values are grouped together.

```sql
SELECT
    department,
    COUNT(*)
FROM Employees
GROUP BY department;
```

All rows with `NULL` in `department` belong to the same group.

---

## `ORDER BY` Columns Not in `SELECT`

Many SQL databases allow sorting by columns that are not returned.

```sql
SELECT
    name
FROM Employees
ORDER BY salary DESC;
```

This behavior is supported by many database systems, though specific restrictions may apply depending on the SQL dialect and query type.

---

## Window Functions and `WHERE`

Window functions are evaluated after `WHERE`.

Incorrect:

```sql
SELECT
    name,
    ROW_NUMBER() OVER (ORDER BY salary DESC) AS row_num
FROM Employees
WHERE row_num = 1;
```

Correct:

```sql
SELECT *
FROM (
    SELECT
        name,
        ROW_NUMBER() OVER (ORDER BY salary DESC) AS row_num
    FROM Employees
) AS ranked
WHERE row_num = 1;
```

---

# Key Points

- SQL is written top-to-bottom but executed in a logical order.
    
- `FROM` and `JOIN` create the working dataset.
    
- `WHERE` filters individual rows before grouping.
    
- `GROUP BY` creates summary groups.
    
- `HAVING` filters groups after aggregation.
    
- `SELECT` determines the final output columns.
    
- `ORDER BY` sorts the final result set.
    
- `JOIN` combines normalized tables using matching keys.
    
- `CASE WHEN` provides conditional logic within queries.
    
- Window functions compute rankings and analytics without reducing row count.
    

---

# Interview Summary

### Explain the SQL Execution Order

```
FROM
↓

JOIN

↓

WHERE

↓

GROUP BY

↓

HAVING

↓

SELECT

↓

ORDER BY
```

---

### Frequently Asked Interview Questions

**Q1. Why can't aggregate functions be used in `WHERE`?**

Because `WHERE` executes before `GROUP BY`, aggregate values do not exist yet.

---

**Q2. What is the difference between `WHERE` and `HAVING`?**

- `WHERE` filters rows before grouping.
    
- `HAVING` filters groups after aggregation.
    

---

**Q3. Why is understanding execution order important?**

It helps explain SQL errors, improves query design, and enables more efficient and predictable queries.

---

**Q4. When should you use a window function instead of `GROUP BY`?**

Use a window function when you need calculations such as rankings, running totals, or averages **while keeping every original row**. Use `GROUP BY` when you want summarized results with fewer rows.

---

**Q5. What is the difference between `ROW_NUMBER()` and `RANK()`?**

- `ROW_NUMBER()` assigns a unique sequential number to every row.
    
- `RANK()` gives equal values the same rank and skips subsequent rank numbers after ties.
    

---

**Q6. What happens if a `JOIN` is written without an `ON` clause (where required)?**

The query may produce a Cartesian product, combining every row from one table with every row from the other, often resulting in an unexpectedly large result set.