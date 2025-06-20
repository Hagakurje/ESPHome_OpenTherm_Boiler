import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import api
from esphome.const import CONF_ID

CODEOWNERS = ["@your-github-handle"]

opentherm_ns = cg.esphome_ns.namespace("opentherm")
OpenThermComponent = opentherm_ns.class_("OpenThermComponent", cg.Component, cg.PollingComponent)

CONF_IN_PIN = "in_pin"
CONF_OUT_PIN = "out_pin"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(OpenThermComponent),
    cv.Required(CONF_IN_PIN): cv.int_,
    cv.Required(CONF_OUT_PIN): cv.int_,
}).extend(cv.polling_component_schema("1s"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_IN_PIN], config[CONF_OUT_PIN])
    await cg.register_component(var, config)