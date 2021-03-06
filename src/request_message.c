#include <czmq.h>
#include "request_message.h"
#include "response_message.h"

enum
{
    PROP_0,
    
    IPCAM_REQUEST_MESSAGE_ACTION = 1,
    IPCAM_REQUEST_MESSAGE_ID = 2,
    
    N_PROPERTIES
};

typedef struct _IpcamRequestMessagePrivate
{
    gchar *action;
    gchar *id;
} IpcamRequestMessagePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(IpcamRequestMessage, ipcam_request_message, IPCAM_MESSAGE_TYPE);

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL, };

static void ipcam_request_message_dispose(GObject *self)
{
    static gboolean first_run = TRUE;
    if (first_run)
    {
        first_run = FALSE;
        G_OBJECT_CLASS(ipcam_request_message_parent_class)->dispose(self);
    }
}
static void ipcam_request_message_finalize(GObject *self)
{
    IpcamRequestMessagePrivate *priv = ipcam_request_message_get_instance_private(IPCAM_REQUEST_MESSAGE(self));
    g_free(priv->action);
    g_free(priv->id);
    G_OBJECT_CLASS(ipcam_request_message_parent_class)->finalize(self);
}
static void ipcam_request_message_get_property(GObject *object,
                                               guint property_id,
                                               GValue *value,
                                               GParamSpec *pspec)
{
    IpcamRequestMessage *self = IPCAM_REQUEST_MESSAGE(object);
    IpcamRequestMessagePrivate *priv = ipcam_request_message_get_instance_private(self);
    switch(property_id)
    {
    case IPCAM_REQUEST_MESSAGE_ACTION:
        {
            g_value_set_string(value, priv->action);
        }
        break;
    case IPCAM_REQUEST_MESSAGE_ID:
        {
            g_value_set_string(value, priv->id);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_request_message_set_property(GObject *object,
                                               guint property_id,
                                               const GValue *value,
                                               GParamSpec *pspec)
{
    IpcamRequestMessage *self = IPCAM_REQUEST_MESSAGE(object);
    IpcamRequestMessagePrivate *priv = ipcam_request_message_get_instance_private(self);
    switch(property_id)
    {
    case IPCAM_REQUEST_MESSAGE_ACTION:
        {
            g_free(priv->action);
            priv->action = g_value_dup_string(value);
            /* g_print("ipcam request message action: %s\n", priv->action); */
        }
        break;
    case IPCAM_REQUEST_MESSAGE_ID:
        {
            g_free(priv->id);
            priv->id = g_value_dup_string(value);
            /* g_print("ipcam request message id: %s\n", priv->id); */
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}
static void ipcam_request_message_init(IpcamRequestMessage *self)
{
    zuuid_t *uuid = zuuid_new();
    gchar *message_id = zuuid_str(uuid);
    g_object_set(G_OBJECT(self), "type", "request", "id", message_id, "action", "", NULL);
    zuuid_destroy(&uuid);
}
static void ipcam_request_message_class_init(IpcamRequestMessageClass *klass)
{
    GObjectClass *this_class = G_OBJECT_CLASS(klass);

    this_class->dispose = &ipcam_request_message_dispose;
    this_class->finalize = &ipcam_request_message_finalize;
    this_class->get_property = &ipcam_request_message_get_property;
    this_class->set_property = &ipcam_request_message_set_property;

    obj_properties[IPCAM_REQUEST_MESSAGE_ACTION] =
        g_param_spec_string("action",
                            "Request message action",
                            "Set request message action",
                            "", // default value
                            G_PARAM_READWRITE);
    obj_properties[IPCAM_REQUEST_MESSAGE_ID] =
        g_param_spec_string("id",
                            "Request message id",
                            "Set request message id",
                            "", // default value
                            G_PARAM_READWRITE);
    
    g_object_class_install_properties(this_class, N_PROPERTIES, obj_properties);
}
IpcamMessage *ipcam_request_message_get_response_message(IpcamRequestMessage *request_message, const gchar *code)
{
    IpcamRequestMessagePrivate *priv = ipcam_request_message_get_instance_private(request_message);
    IpcamMessage *message = g_object_new(IPCAM_RESPONSE_MESSAGE_TYPE,
                                         "action", priv->action,
                                         "id", priv->id,
                                         "code", code,
                                         NULL);
    return message;
}

const gchar *ipcam_request_message_get_action(IpcamRequestMessage *request_message)
{
	IpcamRequestMessagePrivate *priv = ipcam_request_message_get_instance_private(request_message);

	return priv->action;
}

const gchar *ipcam_request_message_get_id(IpcamRequestMessage *request_message)
{
	IpcamRequestMessagePrivate *priv = ipcam_request_message_get_instance_private(request_message);

	return priv->id;
}
