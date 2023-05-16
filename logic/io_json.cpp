#include "io.h"

void IOJSON::load(QString file_name) {
    QFile json_file(file_name);

    if (!(json_file.open(QIODevice::ReadOnly | QIODevice::Text)))
        throw std::runtime_error("Cannot open json file: " + file_name.toStdString());

    auto json = QJsonDocument::fromJson(json_file.readAll());

    QJsonArray variables_array;
    if (json.isArray())
        variables_array = json.array();
    else
        throw std::runtime_error("Json file must contain array with variables");

    QList<VariableData> variables;
    for (auto && i : variables_array) {
        QJsonObject variable_object = i.toObject();

        // check structure of field "name"
        if (!variable_object.contains("name"))
            throw std::runtime_error("The variable object must contain field \"name\".");
        if (!variable_object["name"].isObject())
            throw std::runtime_error("Name field must be object.");

        QJsonObject naming_object = variable_object["name"].toObject();
        if (!(naming_object.contains("full") && naming_object.contains("alias")))
            throw std::runtime_error(R"(Field "name" must contain fields "full" and "alias".)");
        if (!(naming_object["full"].isString() && naming_object["alias"].isString()))
            throw std::runtime_error(R"(Fields "full" and "alias" in field "name" must be strings)");

        // check structure of field "instrument"
        if (!variable_object.contains("instrument"))
            throw std::runtime_error("Variable object must contain field \"instrument\".");
        if (!variable_object["instrument"].isObject())
            throw std::runtime_error("Field \"instrument\" must be object.");

        QJsonObject instrument_object = variable_object["instrument"].toObject();
        if (!(instrument_object.contains("error_type") && instrument_object.contains("error_value")))
            throw std::runtime_error(R"(Instrument field must contain fields "error_type" and "error_value".)");

        if (!instrument_object["error_type"].isString())
            throw std::runtime_error("Field \"error_type\" must be string.");

        QString error_type = instrument_object["error_type"].toString();
        if (!QList<QString>{"Absolute", "Relative", "Calculated"}.contains(error_type))
            throw std::runtime_error(R"("Field "error_type" must be "Absolute", "Relative" or "Calculated".")");

        if (error_type == "Calculated" && !instrument_object["error_value"].isArray())
            throw std::runtime_error(R"(Field "error_value" when "error_type" equal "Calculated" must be array)");
        else if (error_type != "Calculated" && !instrument_object["error_value"].isDouble())
            throw std::runtime_error(R"("Field "error_value" when "error_type" equal "Absolute" or "Relative" must be double.")");

        if (instrument_object["error_value"].isDouble() && instrument_object["error_value"].toDouble() < 0)
            throw std::runtime_error(R"("Field "error_value" must be positive.")");

        // check measurements
        if (!variable_object.contains("measurements"))
            throw std::runtime_error("The variable object must contain field \"measurements\".");
        if (!variable_object["measurements"].isArray())
            throw std::runtime_error("Measurement field must be array.");

        QJsonArray measurements_array = variable_object["measurements"].toArray();
        QList<double> measurements;

        for (auto &&j : measurements_array)
            measurements.append(j.toDouble());


        Instrument instrument;
        Instrument calculated_instrument(QList<double>{});
        VariableData variable;

        variable.naming.full = naming_object["full"].toString();
        variable.naming.alias = naming_object["alias"].toString();

        if (error_type == "Calculated") {
            Instrument tmp_instrument(QList<double>{});
            instrument.error.type = ErrorType::Calculated;
            for (auto &&j : instrument_object["error_value"].toArray())
                instrument.error.list.append(j.toDouble());

            if (measurements.size() != instrument.error.list.size())
                throw std::runtime_error(R"("Field "measurements" and "error_value" must be same size.")");
            instrument = tmp_instrument;
        } else if (error_type == "Relative") {
            instrument.error.type = ErrorType::Relative;
            instrument.error.value = instrument_object["error_value"].toDouble();
        } else if (error_type == "Absolute") {
            instrument.error.type = ErrorType::Absolute;
            instrument.error.value = instrument_object["error_value"].toDouble();
        }

        if (instrument_object.contains("name")) {
            if (!instrument_object["name"].isString())
                throw std::runtime_error(R"("Field "name" in field "instrument" must be string.")");
            instrument.name = instrument_object["name"].toString();
        }

        variable.measurements.swap(measurements);
        variable.instrument = instrument;
        variables.append(variable);
    }

    for (const auto &variable : variables)
        Manager::instance()->AddVariable(variable);
}

QJsonDocument IOJSON::save() {
    QJsonDocument json;
    QJsonArray variables;

    for (const auto &variable : Manager::instance()->variables) {
        QVariantMap variable_map;

        variable_map["name"] = QMap<QString, QVariant>{{"full", variable.naming.full}, {"alias", variable.naming.alias}};
        variable_map["measurements"] = QList<QVariant>{variable.measurements.begin(), variable.measurements.end()};
        QVariantMap instrument;

        instrument["name"] = variable.instrument.name;

        switch (variable.instrument.error.type) {
            case ErrorType::Calculated:
                instrument["error_type"] = "Calculated";
                instrument["error_value"] = QList<QVariant>(variable.instrument.error.list.begin(), variable.instrument.error.list.end());
                break;
            case ErrorType::Absolute:
                instrument["error_type"] =  "Absolute";
                instrument["error_value"] =  variable.instrument.error.value;
                break;
            case ErrorType::Relative:
                instrument["error_type"] =  "Relative";
                instrument["error_value"] = variable.instrument.error.value;
                break;
        }
        variable_map["instrument"] = instrument;
        variables.append(QJsonObject::fromVariantMap(variable_map));
    }
    json.setArray(variables);
    return json;
}