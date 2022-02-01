# Datax

datax is a file format. if JSON and INI had a deformed baby this would be it.

## Datax showcase

```yaml
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
    dtx::Reader reader(dtx::From::File, "test.datax");

    if(!reader.state.ok)
    {
        std::cerr << reader.state.message << '\n';
        return -1;
    }
    
    reader["some header"]["key"] = "value";

    dtx::Writer writer(reader);
    
    std::cout << writer.to_string();
```

### Reader
when you instantiate the Reader class it will read the source either from a file or string and then parse it.

you can use either `dtx::From::File` or `dtx::From::String`.

the reader has a state struct which contains two members `ok` and `message` which are used primarily for reporting errors.

the reader class itself inherits from `std::unordered_map` so you can use it as such.

#### Usage
the file is represented as `dtx::Records` which is just a map of fields.

fields are represented with `dtx::Fields` which is a map of records which is just a struct that contains a varient.

```c++
    dtx::Reader reader(dtx::From::File, "test.datax");

    if(!reader.state.ok)
    {
        std::cerr << reader.state.message << std::endl;
        return -1;
    }
    
    if(!reader.contains("window"))
        init_window_settings(reader);

    bool is_fullscreen = reader["window"]["fullscreen"].get<dtx::Bool>();
    
    if(is_fullscreen)
        std::cout << "is fullscreen";
    else
        std::cout << "is not fullscreen";
```
you can use this to access variants how you would normally.
```c++
dtx::Record record = data["key"];

switch(record.value.index())
{
    case dtx::Type::Number: record.get<dtx::Number>(); break;
    // and so on
}
```

the record types are `Number, Bool, String, Array, Object` and can be used to access a varients underlying type.

these are just small abstractions to make it easier to use varients with the datax reader.

the underlying types of these are `double, bool, std::string, std::vector<Record>, dtx::Fields`

