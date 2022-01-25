# Datax

datax is a file format. if JSON and INI had a deformed baby this would be it.

## Datax showcase

```json
<window>
title: "testing window"
fullscreen: true
width: 1920
height: 1080
state: [ 1, "yes", true, {is_ok: true message: "data is ok"} ]
log_path: { events: "./events.log" crash: "./crash.log" }
```

## Api usage

the C++ api is basically a wrapper around variants and maps

### Example
```c++
    dtx::Reader reader(dtx::From::FILE, "test.datax");

    if(!reader.state.ok)
    {
        std::cerr << reader.state.message << '\n';
        return -1;
    }

    dtx::Writer writer(reader.records);
    
    std::cout << writer.to_string();
```

### Reader
when you instantiate the Reader class it will read the source either from a file or string and then parse it.

you can use either `dtx::From::FILE` or `dtx::From::STRING`.

the reader has a state struct which contains two members `ok` and `message` which are used primarily for reporting errors.

you can access the datax records with the `records` member.

#### Usage
the file is represented as `dtx::Records` which is just a map of fields.

fields are represented with `dtx::Fields` which is a map of records which is just a struct that contains a varient and an enum type.

```c++
    dtx::Reader reader(dtx::From::FILE, "test.datax");

    if(!reader.state.ok)
    {
        std::cerr << reader.state.message << std::endl;
        return -1;
    }
    
    if(!reader.records.contains("window"))
        init_window_settings(reader.records);

    dtx::Fields window_settings = reader.records.at("window");

    dtx::Record fullscreen_record = window_settings.at("fullscreen");
    
    bool is_fullscreen = std::get<bool>(fullscreen_record.value);
    
    if(is_fullscreen)
        std::cout << "is fullscreen";
    else
        std::cout << "is not fullscreen";
```
you can use this to access variants how you would normally.
```c++
dtx::Record some_record = data.at("header");

switch(some_record.type)
{
    case dtx::Type::NUMBER: std::get<double>(some_record.value); break;
    // and so on
}
```

