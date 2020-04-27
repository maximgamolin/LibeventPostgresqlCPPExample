Dependence 

libevent-dev sqlite3


```shell script
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install jansson
```


Проверить запрос в базу

```c++
std::string select_qsl = "SELECT name FROM sqlite_master WHERE type='table'";
    auto cb = [](void *NotUsed, int argc, char **argv, char **azColName){
        int i;
        printf("tables");
        for(i = 0; i<argc; i++) {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        printf("\nend\n");
        return 0;
    };
    sqlite3_exec(context->db, select_qsl.c_str(), cb, nullptr, nullptr);
```

    