# OpenCAPIF Provider Client

This repository contains a client implementation for [OpenCAPIF](https://www.etsi.org/deliver/etsi_gs/CAPIF/), capable of performing all operations typically handled by a CAPIF provider. It includes functionality for provider registration, user authentication, API publishing, and more.

---

## 📦 Prerequisites

Ensure the following dependencies are installed on your system (Ubuntu 22.04 or similar):

```bash
sudo apt update
sudo apt install -y jq make gcc libcurl4-openssl-dev
```

## 📥 Clone the Repository
Clone the repository with submodules to ensure all dependencies are pulled:

```bash
git clone --recurse-submodules https://github.com/aaronmontilla/openCapif.git
cd openCapif
```

## ⚙️ Build the Project
Compile the client by running:

```bash
make
```

## ⚙️ Get certificates
First include the IP address inside /etc/hosts file by adding this line:

"YOUR IP ADDRESS" capifcore register capifhost

Once this is done get the certificates by running:

```bash
./get_certs "Username"
```

## 🚀 Run the OpenCAPIF Process
Start the client:

```bash
./opencapif_process
```
Follow the on-screen instructions to proceed with CAPIF operations.

## 🧩 Available Functions
The client supports a full range of provider-related actions within a CAPIF instance:

- login_admin_curl() – Authenticate as admin

- create_user_curl() – Create a new CAPIF user

- get_auth_curl() – Retrieve access token for a user

- register_provider() – Register a new API provider

- delete_user() – Delete a CAPIF user

- delete_provider() – Remove an API provider

- publish_api() – Publish a new API

- delete_api() – Unpublish an existing API

- retrieve_apis() – List available APIs
