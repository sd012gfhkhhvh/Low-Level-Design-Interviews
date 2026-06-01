# Raw Pointer vs Unique Pointer Ownership

This is one of the most important concepts in modern C++:

> **Ownership is not determined by whether something is a pointer. Ownership is determined by who is responsible for destroying the object.**

---

## Raw Pointer Case

```cpp
EmailChannel email;

NotificationChannel* ptr = &email;
```

Here:

```text
email object  <-- owned by main()
ptr           <-- merely points to email
```

Diagram:

```text
+------------------+
| EmailChannel     |
+------------------+
         ^
         |
         ptr
```

When `ptr` dies:

```cpp
ptr = nullptr;
```

nothing happens to `email`.

Because:

```text
ptr does NOT own email
```

It merely knows where the object is.

---

## unique_ptr Case

```cpp
auto ptr = make_unique<EmailChannel>();
```

Diagram:

```text
unique_ptr ptr
      |
      v
+------------------+
| EmailChannel     |
+------------------+
```

Now:

```text
ptr OWNS EmailChannel
```

When `ptr` dies:

```cpp
{
    auto ptr = make_unique<EmailChannel>();
}
```

the destructor of `unique_ptr` executes:

```cpp
delete managed_object;
```

automatically.

So:

```text
unique_ptr is not just a pointer.
It is an owner object.
```

---

## The Key Difference

### Raw Pointer

```cpp
NotificationChannel* p;
```

stores:

```text
memory address
```

That's all.

It has no ownership semantics.

No destructor behavior.

No automatic cleanup.

---

### unique_ptr

```cpp
unique_ptr<NotificationChannel> p;
```

stores:

```text
memory address
+
ownership policy
+
automatic deletion logic
```

Its destructor roughly does:

```cpp
~unique_ptr() {
    delete ptr;
}
```

---

## Analogy

Imagine a car.

### Raw Pointer

```text
A note with the car's parking location.
```

You know where the car is.

You don't own it.

You aren't responsible for it.

---

### unique_ptr

```text
The car registration papers.
```

You own the car.

When ownership transfers:

```cpp
auto p2 = std::move(p1);
```

the registration papers move to the new owner.

Only one owner exists.

---

## Your NotificationService Example

### Raw Pointer Version

```cpp
EmailChannel email;

vector<NotificationChannel*> channels;
channels.push_back(&email);
```

Ownership:

```text
main()
 └── owns email

NotificationService
 └── borrows email
```

---

### unique_ptr Version

```cpp
vector<unique_ptr<NotificationChannel>> channels;

channels.push_back(
    make_unique<EmailChannel>()
);
```

Ownership:

```text
NotificationService
 └── owns EmailChannel
```

When `NotificationService` dies:

```text
vector destroyed
    ↓
unique_ptr destroyed
    ↓
EmailChannel deleted
```

automatically.

---

## The Golden Mental Model

```text
Raw pointer (*)      = "I know where it is."

Reference (&)        = "I am using it."

unique_ptr<T>        = "I own it."

shared_ptr<T>        = "We jointly own it."
```

That's why **both contain addresses**, but only `unique_ptr` carries the additional contract:

> "I am responsible for deleting this object when I'm done with it."

A raw pointer is just an address; a `unique_ptr` is an address **plus ownership semantics**.
