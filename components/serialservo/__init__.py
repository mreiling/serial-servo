from esphome import automation
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_POSITION, CONF_RX_PIN, CONF_TX_PIN, CONF_SPEED

CODEOWNERS = ["@mreiling"]

CONF_SERVO = "servoid"
CONF_NEWID = "newservoid"

ss_ns = cg.esphome_ns.namespace("serialservo")
SerialServo = ss_ns.class_(
    "SerialServo", cg.Component
)

ServoWriteAction = ss_ns.class_("ServoWriteAction", automation.Action)
ServoSetIDAction = ss_ns.class_("ServoSetIDAction", automation.Action)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SerialServo),
            cv.Required(CONF_RX_PIN): cv.int_range(1,50),
            cv.Required(CONF_TX_PIN): cv.int_range(1,50),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

@automation.register_action(
    "serialservo.write",
    ServoWriteAction,
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(SerialServo),
            cv.Required(CONF_SERVO): cv.int_range(1,253),
            cv.Required(CONF_POSITION):  cv.templatable(cv.int_range(1, 1000)),  
            cv.Required(CONF_SPEED):  cv.int_range(1, 1500),
        }
    ),
)

@automation.register_action(
    "serialservo.setid",
    ServoSetIDAction,
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(SerialServo),
            cv.Required(CONF_SERVO): cv.int_range(1,20),
            cv.Required(CONF_NEWID): cv.int_range(1,20),
        }
    ),
)

async def write_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    cg.add(var.set_servoid(config[CONF_SERVO]))
    template_ = await cg.templatable(config[CONF_POSITION], args, int)
    cg.add(var.set_position(template_))
    cg.add(var.set_speed(config[CONF_SPEED]))
    return var